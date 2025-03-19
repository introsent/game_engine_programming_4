#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <SDL_syswm.h>
#include <steam_api.h>

#include "AchievementObserver.h"
#include "AchievementsGlobals.h"
#include "CharacterFactory.h"
#include "CSteamAchievements.h"
#include "GameObjectBuilder.h"
#include "InputManager.h"
#include "InputState.h"
#include "KillCommand.h"
#include "LivesDisplayObserver.h"
#include "Minigin.h"
#include "MoveCommand.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreCommand.h"
#include "ScoreDisplayObserver.h"
#include "UIFactory.h"


void load()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    // Create background and logo objects.
    auto bg = GameObjectBuilder()
        .WithTexture("background.tga")
        .SetPosition(glm::vec3(0.f, 0.f, 0.f))
        .Build();
    scene.Add(bg);

    auto logo = GameObjectBuilder()
        .WithTexture("logo.tga")
        .SetPosition(glm::vec3(216.f, 180.f, 0.f))
        .Build();
    scene.Add(logo);

    // Load fonts.
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto fontInfo = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);

    // Create UI text elements.
    auto title = CreateUIText("Programming 4 Assignment", font, glm::vec3(80.f, 50.f, 0.f));
    scene.Add(title);

    auto info1 = CreateUIText("Use WASD to move Qbert, C to inflict damage, Z and X to pick up pellets", fontInfo, glm::vec3(0.f, 100.f, 0.f));
    scene.Add(info1);

    auto info2 = CreateUIText("Use D-Pad to move Slick Sam, X to inflict damage, A and B to pick up pellets", fontInfo, glm::vec3(0.f, 120.f, 0.f));
    scene.Add(info2);

    auto fpsText = GameObjectBuilder()
        .WithText("FPS", font)
        .WithFPS()
        .SetPosition(glm::vec3(0.f, 0.f, 0.f))
        .Build();
    scene.Add(fpsText);

    // Create characters using the factory function.
    dae::HealthComponent* healthComponentChar1 = nullptr;
    dae::ScoreComponent* scoreComponentChar1 = nullptr;
    auto char1 = CreateCharacter("qbert_week2.tga", glm::vec3(200.f, 300.f, 0.f),
        &healthComponentChar1, &scoreComponentChar1);
    scene.Add(char1);

    dae::HealthComponent* healthComponentChar2 = nullptr;
    dae::ScoreComponent* scoreComponentChar2 = nullptr;
    auto char2 = CreateCharacter("slick_sam.tga", glm::vec3(200.f, 400.f, 0.f),
        &healthComponentChar2, &scoreComponentChar2);
    scene.Add(char2);

    auto gameManager = GameObjectBuilder().Build();
    auto achievementObserver = gameManager->AddComponent<dae::AchievementObserver>(gameManager.get());
    achievementObserver->Register(scoreComponentChar1);
    achievementObserver->Register(scoreComponentChar2);
    scene.Add(gameManager);

    // Create UI elements for character health and score.
    auto textObjChar1 = CreateUIText("Remaining Lives: 3", fontInfo, glm::vec3(0.f, 200.f, 0.f));
    // Assuming GetComponent<T>() exists to retrieve the text component
    auto textComponentChar1 = textObjChar1->GetComponent<dae::TextComponent>();
    auto livesObserver1 = textObjChar1->AddComponent<dae::LivesDisplayObserver>(textObjChar1.get(), textComponentChar1);
    livesObserver1->Register(healthComponentChar1);
    scene.Add(textObjChar1);

    auto textObjChar2 = CreateUIText("Remaining Lives: 3", fontInfo, glm::vec3(0.f, 250.f, 0.f));
    auto textComponentChar2 = textObjChar2->GetComponent<dae::TextComponent>();
    auto livesObserver2 = textObjChar2->AddComponent<dae::LivesDisplayObserver>(textObjChar2.get(), textComponentChar2);
    livesObserver2->Register(healthComponentChar2);
    scene.Add(textObjChar2);

    auto textObjScoreChar1 = CreateUIText("Score: 0", fontInfo, glm::vec3(0.f, 220.f, 0.f));
    auto textComponentScoreChar1 = textObjScoreChar1->GetComponent<dae::TextComponent>();
    auto scoreObserver1 = textObjScoreChar1->AddComponent<dae::ScoreDisplayObserver>(textObjScoreChar1.get(), textComponentScoreChar1);
    scoreObserver1->Register(scoreComponentChar1);
    scene.Add(textObjScoreChar1);

    auto textObjScoreChar2 = CreateUIText("Score: 0", fontInfo, glm::vec3(0.f, 270.f, 0.f));
    auto textComponentScoreChar2 = textObjScoreChar2->GetComponent<dae::TextComponent>();
    auto scoreObserver2 = textObjScoreChar2->AddComponent<dae::ScoreDisplayObserver>(textObjScoreChar2.get(), textComponentScoreChar2);
    scoreObserver2->Register(scoreComponentChar2);
    scene.Add(textObjScoreChar2);

    // Bind commands to inputs as in your original code.
    float speed1 = 1.0f;
    float speed2 = speed1 * 2.0f;

	// Bind WASD for char1 (keyboard)
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_w, InputState::Pressed,
        std::make_unique<dae::MoveCommand>(char1.get(), speed1, glm::vec2(0.f, -1.f))); // Up
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_s, InputState::Pressed,
        std::make_unique<dae::MoveCommand>(char1.get(), speed1, glm::vec2(0.f, 1.f)));  // Down
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_a, InputState::Pressed,
        std::make_unique<dae::MoveCommand>(char1.get(), speed1, glm::vec2(-1.f, 0.f))); // Left
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_d, InputState::Pressed,
		   std::make_unique<dae::MoveCommand>(char1.get(), speed1, glm::vec2(1.f, 0.f)));  // Right
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_c, InputState::Down,
        std::make_unique<dae::KillCommand>(char1.get()));
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_z, InputState::Down,
        std::make_unique<dae::ScoreCommand>(char1.get(), 10));
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_x, InputState::Down,
        std::make_unique<dae::ScoreCommand>(char1.get(), 100));


	dae::XInputManager m_XInputManager;
	// Bind D-Pad for char2 (controller)
	dae::InputManager::GetInstance().BindControllerCommand(m_XInputManager.GetXInputValue(GamepadButton::DpadUp), InputState::Pressed,
        std::make_unique<dae::MoveCommand>(char2.get(), speed2, glm::vec2(0.f, -1.f)));   // Up
	dae::InputManager::GetInstance().BindControllerCommand(m_XInputManager.GetXInputValue(GamepadButton::DpadDown), InputState::Pressed,
        std::make_unique<dae::MoveCommand>(char2.get(), speed2, glm::vec2(0.f, 1.f)));    // Down
	dae::InputManager::GetInstance().BindControllerCommand(m_XInputManager.GetXInputValue(GamepadButton::DpadLeft), InputState::Pressed,
        std::make_unique<dae::MoveCommand>(char2.get(), speed2, glm::vec2(-1.f, 0.f)));   // Left
	dae::InputManager::GetInstance().BindControllerCommand(m_XInputManager.GetXInputValue(GamepadButton::DpadRight), InputState::Pressed,
        std::make_unique<dae::MoveCommand>(char2.get(), speed2, glm::vec2(1.f, 0.f)));    // Right
	
	dae::InputManager::GetInstance().BindControllerCommand(m_XInputManager.GetXInputValue(GamepadButton::West), InputState::Up,
        std::make_unique<dae::KillCommand>(char2.get()));
	dae::InputManager::GetInstance().BindControllerCommand(m_XInputManager.GetXInputValue(GamepadButton::South), InputState::Up,
        std::make_unique<dae::ScoreCommand>(char2.get(), 10));
	dae::InputManager::GetInstance().BindControllerCommand(m_XInputManager.GetXInputValue(GamepadButton::East), InputState::Up,
        std::make_unique<dae::ScoreCommand>(char2.get(), 100));

	//auto exercise1 = std::make_shared<dae::GameObject>();
	//exercise1->AddComponent<dae::IntegerBufferBenchmarkWindowComponent>(exercise1.get());
	//exercise1->SetLocalPosition(glm::vec3(80.f, 50.f, 0.f));
	//scene.Add(exercise1);
	//
	//auto exercise2 = std::make_shared<dae::GameObject>();
	//exercise1->AddComponent<dae::StructVsPointerBenchmarkWindowComponent>(exercise2.get());
	//exercise1->SetLocalPosition(glm::vec3(200.f, 50.f, 0.f));
	//scene.Add(exercise2);
	
	//--Checked--
	//char1->RemoveChild(char2.get());

	//--Checked--
	//char1->RemoveComponent<dae::RotationComponent>();
}


int main(int, char*[]) {
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else
		std::cout << "Successfully initialized steam." << std::endl;

	g_SteamAchievements = new CSteamAchievements(g_Achievements.data(), static_cast<int>(g_Achievements.size()));

	dae::Minigin engine("../Data/");
	engine.Run(load);

	// Clean up our Steam Achievements system.
	delete g_SteamAchievements;
	g_SteamAchievements = nullptr;

	SteamAPI_Shutdown();
	return 0;
}


