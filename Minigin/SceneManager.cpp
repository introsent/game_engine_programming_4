#include "SceneManager.h"
#include <iostream>
#include "Scene.h"

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (auto& scene : m_scenes)
	{
		if (scene->GetName() == name)
		{
			m_pActiveScene = scene.get();
			return;
		}
	}

	std::cerr << "Scene '" << name << "' not found!\n";
	m_pActiveScene = nullptr;
}

dae::Scene* dae::SceneManager::GetActiveScene() const
{
	return m_pActiveScene;
}


void dae::SceneManager::Update(float deltaTime)
{
	if (m_pActiveScene)
	{
		m_pActiveScene->Update(deltaTime);
	}
}

void dae::SceneManager::Render() const
{
	if (m_pActiveScene)
		m_pActiveScene->Render();
}

void dae::SceneManager::RenderUI()
{
	if (m_pActiveScene)
		m_pActiveScene->RenderUI();
}


dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
