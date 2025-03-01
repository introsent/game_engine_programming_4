#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "FPSComponent.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "IntegerBufferBenchmarkWindowComponent.h"
#include "RotationComponent.h"
#include "Scene.h"
#include "StructVsPointerBenchmarkWindowComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"

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

	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextComponent>(to.get(), "Programming 4 Assignment", font);
	to->SetLocalPosition(glm::vec3(80.f, 50.f, 0.f));
	scene.Add(to);

	to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextComponent>(to.get(), "FPS", font);
	to->AddComponent<dae::FPSComponent>(to.get());
	to->SetLocalPosition(glm::vec3(0.f, 0.f, 0.f));
	scene.Add(to);

	auto posToOrbit = std::make_shared<dae::GameObject>();
	posToOrbit->SetLocalPosition(glm::vec3(200.f, 300.f, 0.f));
	scene.Add(posToOrbit);

	auto char1 = std::make_shared<dae::GameObject>();
	char1->AddComponent<dae::TextureComponent>(char1.get(), "qbert_week2.tga");
	char1->AddComponent<dae::RotationComponent>(char1.get(), 20.f, -500.f);
	char1->SetParent(posToOrbit.get(), false);
	scene.Add(char1);

	auto char2 = std::make_shared<dae::GameObject>();
	char2->AddComponent<dae::TextureComponent>(char2.get(), "qbert_week2.tga");
	char2->AddComponent<dae::RotationComponent>(char2.get(), 30.f, 300.f);
	char2->SetParent(char1.get(), false);
	scene.Add(char2);

	auto exercise1 = std::make_shared<dae::GameObject>();
	exercise1->AddComponent<dae::IntegerBufferBenchmarkWindowComponent>(exercise1.get());
	exercise1->SetLocalPosition(glm::vec3(80.f, 50.f, 0.f));
	scene.Add(exercise1);

	auto exercise2 = std::make_shared<dae::GameObject>();
	exercise1->AddComponent<dae::StructVsPointerBenchmarkWindowComponent>(exercise2.get());
	exercise1->SetLocalPosition(glm::vec3(200.f, 50.f, 0.f));
	scene.Add(exercise2);
	
	//--Checked--
	//char1->RemoveChild(char2.get());

	//--Checked--
	//char1->RemoveComponent<dae::RotationComponent>();
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}