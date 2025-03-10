#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <SDL_syswm.h>
#include <XInput.h>

#include "FPSComponent.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "InputManager.h"
#include "IntegerBufferBenchmarkWindowComponent.h"
#include "MoveCommand.h"
#include "RotationComponent.h"
#include "Scene.h"
#include "StructVsPointerBenchmarkWindowComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "TranslationComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>(go.get(), "background.tga");
	go->SetLocalPosition(glm::vec3(0.f, 0.f, 0.f));
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>(go.get(), "logo.tga");
	go->SetLocalPosition(glm::vec3(216.f, 180.f, 0.f));
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto fontInfo = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);

	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextComponent>(to.get(), "Programming 4 Assignment", font);
	to->SetLocalPosition(glm::vec3(80.f, 50.f, 0.f));
	scene.Add(to);

	auto info1 = std::make_shared<dae::GameObject>();
	info1->AddComponent<dae::TextComponent>(info1.get(), "Use WASD to move Qbert", fontInfo);
	info1->SetLocalPosition(glm::vec3(0.f, 100.f, 0.f));
	scene.Add(info1);

	auto info2 = std::make_shared<dae::GameObject>();
	info2->AddComponent<dae::TextComponent>(info2.get(), "Use D-Pad to move Slick Sam", fontInfo);
	info2->SetLocalPosition(glm::vec3(0.f, 120.f, 0.f));
	scene.Add(info2);

	to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextComponent>(to.get(), "FPS", font);
	to->AddComponent<dae::FPSComponent>(to.get());
	to->SetLocalPosition(glm::vec3(0.f, 0.f, 0.f));
	scene.Add(to);

	auto posToOrbit = std::make_shared<dae::GameObject>();
	posToOrbit->SetLocalPosition(glm::vec3(200.f, 300.f, 0.f));
	scene.Add(posToOrbit);

	// Create first character
	auto char1 = std::make_shared<dae::GameObject>();
	char1->AddComponent<dae::TextureComponent>(char1.get(), "qbert_week2.tga");
	char1->AddComponent<dae::TranslationComponent>(char1.get());
	char1->SetLocalPosition(glm::vec3(200.f, 300.f, 0.f));
	scene.Add(char1);

	// Create second character
	auto char2 = std::make_shared<dae::GameObject>();
	char2->AddComponent<dae::TextureComponent>(char2.get(), "slick_sam.tga");
	char2->AddComponent<dae::TranslationComponent>(char2.get());
	char2->SetLocalPosition(glm::vec3(200.f, 400.f, 0.f));
	scene.Add(char2);

	// Define speeds: char2 moves at double speed of char1.
	float speed1 = 1.0f;
	float speed2 = speed1 * 2.0f;

	// Bind WASD for char1 (keyboard)
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_w, InputState::Pressed,
		new dae::MoveCommand(char1.get(), speed1, glm::vec2(0.f, -1.f))); // Up
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_s, InputState::Pressed,
		new dae::MoveCommand(char1.get(), speed1, glm::vec2(0.f, 1.f)));  // Down
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_a, InputState::Pressed,
		new dae::MoveCommand(char1.get(), speed1, glm::vec2(-1.f, 0.f))); // Left
	dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_d, InputState::Pressed,
		new dae::MoveCommand(char1.get(), speed1, glm::vec2(1.f, 0.f)));  // Right

	// Bind D-Pad for char2 (controller)
	dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP, InputState::Pressed,
		new dae::MoveCommand(char2.get(), speed2, glm::vec2(0.f, -1.f)));   // Up
	dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN, InputState::Pressed,
		new dae::MoveCommand(char2.get(), speed2, glm::vec2(0.f, 1.f)));    // Down
	dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT, InputState::Pressed,
		new dae::MoveCommand(char2.get(), speed2, glm::vec2(-1.f, 0.f)));   // Left
	dae::InputManager::GetInstance().BindControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT, InputState::Pressed,
		new dae::MoveCommand(char2.get(), speed2, glm::vec2(1.f, 0.f)));    // Right

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
	//AllocConsole();
	//
	//FILE* stream;
	//freopen_s(&stream, "CONIN$", "r", stdin);
	//freopen_s(&stream, "CONOUT$", "w", stdout);
	//freopen_s(&stream, "CONOUT$", "w", stderr);


	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}