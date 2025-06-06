#include "SoundService.h"
#include <SDL_mixer.h>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <ranges>
#include <unordered_map>

#include "Command.h"
#include "LoadSoundCommand.h"
#include "PlaySoundCommand.h"

namespace dae
{
    class SoundService::MixerImpl {
        std::mutex m_queueMutex;
        std::condition_variable m_queueCV;
        std::queue<std::unique_ptr<Command>> m_commandQueue;
        std::jthread m_workerThread;
        std::unordered_map<std::string, Mix_Chunk*> m_sounds;
        bool m_running = true;

        void WorkerLoop() {
            while (m_running) {
                std::queue<std::unique_ptr<Command>> localQueue;

                {
                    std::unique_lock lock(m_queueMutex);
                    m_queueCV.wait(lock, [this] {
                        return !m_commandQueue.empty() || !m_running;
                        });

                    if (!m_running) break;

                    std::swap(localQueue, m_commandQueue);
                }

                while (!localQueue.empty()) {
                    auto cmd = std::move(localQueue.front());
                    localQueue.pop();
                    cmd->Execute();
                }
            }
        }

    public:
        MixerImpl() {
            Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
            m_workerThread = std::jthread([this] { WorkerLoop(); });
        }

        ~MixerImpl() {
            {
                std::lock_guard lock(m_queueMutex);
                m_running = false;
            }
            m_queueCV.notify_all();
            m_workerThread.join();

            for (auto& chunk : m_sounds | std::views::values)
                Mix_FreeChunk(chunk);
            Mix_CloseAudio();
        }
        MixerImpl(const MixerImpl&) = delete;
        MixerImpl& operator=(const MixerImpl&) = delete;
        MixerImpl(MixerImpl&&) = delete;
        MixerImpl& operator=(MixerImpl&&) = delete;

        void LoadSound(const std::string& id, const std::string& path) {
            std::lock_guard lock(m_queueMutex);
            m_commandQueue.push(std::make_unique<LoadSoundCommand>(id, path, m_sounds));
            m_queueCV.notify_one();
        }

        void PlaySound(const std::string& id) {
            std::lock_guard lock(m_queueMutex);
            m_commandQueue.push(std::make_unique<PlaySoundCommand>(id, m_sounds));
            m_queueCV.notify_one();
        }
    };
}


dae::SoundService::SoundService()
    : m_mixerPimpl(std::make_unique<MixerImpl>()) {
}

dae::SoundService::~SoundService() = default;

void dae::SoundService::LoadSound(const std::string& id, const std::string& path) {
    m_mixerPimpl->LoadSound(id, path);
}

void dae::SoundService::PlaySound(const std::string& id) {
    m_mixerPimpl->PlaySound(id);
}