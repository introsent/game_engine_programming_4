#include "SceneManager.h"
#include <iostream>
#include "Minigin.h"
#include "CharacterFactory.h"
#include "GameObjectBuilder.h"
#include "InputManager.h"
#include "InputState.h"
#include "KillCommand.h"
#include "LevelComponent.h"
#include "LivesDisplayObserver.h"
#include "MoveCommand.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ScoreCommand.h"
#include "ScoreDisplayObserver.h"
#include "SoundService.h"
#include "SoundServiceLocator.h"
#include "TestSoundCommand.h"
#include "UIFactory.h"
#include "LevelManagerComponent.h"
#include "HighscoreLoader.h"
#include "NameEntryComponent.h"
#include "MenuComponent.h"
#include "MenuCommands.h"
#include "NameEntryCommands.h"
#include "SkipLevelCommand.h"
#include "MuteSoundCommand.h"

void SceneManager::CreateGameScene(GameMode mode, const std::vector<std::string>& playerNames)
{
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    dae::SoundServiceLocator::Register(std::make_unique<dae::SoundService>());
    dae::SoundServiceLocator::GetService()->RegisterSound("qbert_jump",         "Data/Sounds/QBert Jump.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("qbert_hit",          "Data/Sounds/Qbert Hit.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("qbert_fall",         "Data/Sounds/QBert Fall.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("round_complete",     "Data/Sounds/Round Complete Tune.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("swearing",           "Data/Sounds/Swearing.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("coily_snake_jump",   "Data/Sounds/Coily Snake Jump.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("coily_egg_jump",     "Data/Sounds/Coily Egg Jump.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("coily_fall",         "Data/Sounds/Coily Fall.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("others_jump",        "Data/Sounds/Other Foes Jump.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("disk_lift",          "Data/Sounds/Disk Lift.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("disk_land",          "Data/Sounds/Disk Land.wav");
    dae::SoundServiceLocator::GetService()->RegisterSound("slick_sam_caught",   "Data/Sounds/SlickSam Caught.wav");

    auto& scene = dae::SceneManager::GetInstance().CreateScene("Main");
    dae::SceneManager::GetInstance().SetActiveScene("Main");

    auto bg = GameObjectBuilder()
        .WithTexture("background.png", -2.f, 1.f)
        .SetPosition(glm::vec3(0.f, 0.f, 0.f))
        .Build();
    scene.Add(std::move(bg));

    auto playerDataObj = std::make_unique<dae::GameObject>();
    auto playerData = playerDataObj->AddComponent<PlayerDataComponent>(playerDataObj.get());
    if (mode == GameMode::Coop) {
        playerData->SetNickname(playerNames[0] + " & " + playerNames[1]);
    }
    else {
        playerData->SetNickname(playerNames[0]);
    }
    scene.Add(std::move(playerDataObj));

    auto manager = std::make_unique<dae::GameObject>();
    auto levelManager = manager->AddComponent<LevelManagerComponent>(manager.get(), playerData, mode);
    levelManager->OnAllLevelsCompleted.Subscribe([mode, playerData]()
        {
            HandleSwitchingToHighscoreScene(mode, playerData);
        }
    );
    dae::InputManager::GetInstance().BindKeyboardCommand(
        SDLK_F1, InputState::Down,
        std::make_unique<SkipLevelCommand>(levelManager)
    );
    dae::InputManager::GetInstance().BindKeyboardCommand(
        SDLK_F2, InputState::Down,
        std::make_unique<MuteSoundCommand>()
    );
    scene.Add(std::move(manager));

    auto font = dae::ResourceManager::GetInstance().LoadFont("Minecraft.ttf", 36);
    auto scoreText = CreateUIText("Score: 0", font, glm::vec3(10, 10, 0));
    auto scoreObserver = scoreText->AddComponent<ScoreDisplayObserver>(scoreText.get(), scoreText->GetComponent<dae::TextComponent>());
    scoreObserver->Register(playerData->GetScore());
    scene.Add(std::move(scoreText));

    auto livesText = CreateUIText("Lives: 3", font, glm::vec3(10, 50, 0));
    auto livesObserver = livesText->AddComponent<LivesDisplayObserver>(livesText.get(), livesText->GetComponent<dae::TextComponent>());
    livesObserver->Register(playerData->GetHealth());
    scene.Add(std::move(livesText));

    std::string nameDisplay = mode == GameMode::Coop ? "Players: " + playerNames[0] + " & " + playerNames[1] : "Player: " + playerNames[0];
    auto nameText = CreateUIText(nameDisplay, font, glm::vec3(10, 90, 0));
    scene.Add(std::move(nameText));

    auto health = playerData->GetHealth();
    health->OnHealthChanged.Subscribe([mode, health, playerData]() {
        if (health->GetLives() < 0) {
            HandleSwitchingToHighscoreScene(mode, playerData);
        }
        });
}

void SceneManager::HandleSwitchingToHighscoreScene(GameMode mode, PlayerDataComponent* playerData)
{
    std::string name = playerData->GetNickname();
    int score = playerData->GetScore()->GetScore();
    HighscoreLoader::SaveHighScore(mode, name, score);
    CreateHighscoreScene(score, mode);
}

void SceneManager::CreateNameEntryScene(GameMode mode)
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("NameEntry");
    dae::SceneManager::GetInstance().SetActiveScene("NameEntry");

    auto bg = GameObjectBuilder()
        .WithTexture("background.png", -2.f, 1.f)
        .SetPosition(glm::vec3(0.f, 0.f, 0.f))
        .Build();
    scene.Add(std::move(bg));

    auto font = dae::ResourceManager::GetInstance().LoadFont("Minecraft.ttf", 36);
    auto title = CreateUIText("Enter Your Name", font, glm::vec3(150, 100, 0));
    scene.Add(std::move(title));

    std::vector<dae::GameObject*> slotGOs;
    for (int i = 0; i < 5; ++i) {
        auto slot = CreateUIText(" ", font, glm::vec3(200 + i * 50, 200, 0));
        slotGOs.push_back(slot.get());
        scene.Add(std::move(slot));
    }

    auto doneText = CreateUIText("Done", font, glm::vec3(200, 250, 0));
    auto* doneTextPtr = doneText.get();
    scene.Add(std::move(doneText));

    auto warningText = CreateUIText(" ", font, glm::vec3(150, 300, 0));
    auto* warningTextPtr = warningText.get();
    scene.Add(std::move(warningText));

    auto arrow = GameObjectBuilder()
        .WithTexture("Selection Arrow.png", 0.f, 3.f)
        .SetPosition(glm::vec3(180, 200, 0))
        .Build();

    auto* arrowPtr = arrow.get();
    scene.Add(std::move(arrow));

    auto nameEntryGO = std::make_unique<dae::GameObject>();
    auto nameEntryComp = nameEntryGO->AddComponent<NameEntryComponent>(nameEntryGO.get(), mode);
    nameEntryComp->SetSlotTexts(slotGOs);
    nameEntryComp->SetDoneText(doneTextPtr);
    nameEntryComp->SetArrow(arrowPtr);
    nameEntryComp->SetWarningText(warningTextPtr);
    scene.Add(std::move(nameEntryGO));

    dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_w, InputState::Down, std::make_unique<NameEntryUpCommand>(nameEntryComp));
    dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_s, InputState::Down, std::make_unique<NameEntryDownCommand>(nameEntryComp));
    dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_a, InputState::Down, std::make_unique<NameEntryLeftCommand>(nameEntryComp));
    dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_d, InputState::Down, std::make_unique<NameEntryRightCommand>(nameEntryComp));
    dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_RETURN, InputState::Down, std::make_unique<NameEntrySelectCommand>(nameEntryComp));

    dae::InputManager::GetInstance().BindControllerCommand(0, dae::XInputManager::GetXInputValue(GamepadButton::DpadUp), InputState::Down, std::make_unique<NameEntryUpCommand>(nameEntryComp));
    dae::InputManager::GetInstance().BindControllerCommand(0, dae::XInputManager::GetXInputValue(GamepadButton::DpadDown), InputState::Down, std::make_unique<NameEntryDownCommand>(nameEntryComp));
    dae::InputManager::GetInstance().BindControllerCommand(0, dae::XInputManager::GetXInputValue(GamepadButton::DpadLeft), InputState::Down, std::make_unique<NameEntryLeftCommand>(nameEntryComp));
    dae::InputManager::GetInstance().BindControllerCommand(0, dae::XInputManager::GetXInputValue(GamepadButton::DpadRight), InputState::Down, std::make_unique<NameEntryRightCommand>(nameEntryComp));
    dae::InputManager::GetInstance().BindControllerCommand(0, dae::XInputManager::GetXInputValue(GamepadButton::South), InputState::Down, std::make_unique<NameEntrySelectCommand>(nameEntryComp));
}

void SceneManager::CreateMenuScene()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Menu");
    dae::SceneManager::GetInstance().SetActiveScene("Menu");

    auto bg = GameObjectBuilder()
        .WithTexture("background.png", -2.f, 1.f)
        .SetPosition(glm::vec3(0.f, 0.f, 0.f))
        .Build();
    scene.Add(std::move(bg));

    auto font = dae::ResourceManager::GetInstance().LoadFont("Minecraft.ttf", 72);
    auto title = CreateUIText("QBERT", font, glm::vec3(175, 100, 0));
    scene.Add(std::move(title));

    font = dae::ResourceManager::GetInstance().LoadFont("Minecraft.ttf", 36);
    auto soloText = CreateUIText("Solo Mode", font, glm::vec3(220, 250, 0));
    auto coopText = CreateUIText("Co-op Mode", font, glm::vec3(220, 320, 0));
    auto versusText = CreateUIText("Versus Mode", font, glm::vec3(220, 390, 0));

    auto arrow = GameObjectBuilder()
        .WithTexture("Selection Arrow.png", 0.f, 3.f)
        .SetPosition(glm::vec3(270, 250, 0))
        .Build();

    auto menuController = std::make_unique<dae::GameObject>();
    auto menuComp = menuController->AddComponent<MenuComponent>(menuController.get());

    auto* soloPtr = soloText.get();
    auto* coopPtr = coopText.get();
    auto* versusPtr = versusText.get();

    menuComp->SetMenuItems({ soloPtr, coopPtr, versusPtr });
    menuComp->SetArrow(arrow.get());

    menuComp->AddMenuItem("Solo", []() {
        CreateNameEntryScene(GameMode::Solo);
        });
    menuComp->AddMenuItem("Co-op", []() {
        CreateNameEntryScene(GameMode::Coop);
        });
    menuComp->AddMenuItem("Versus", []() {
        CreateNameEntryScene(GameMode::Versus);
        });

    scene.Add(std::move(soloText));
    scene.Add(std::move(coopText));
    scene.Add(std::move(versusText));
    scene.Add(std::move(arrow));
    scene.Add(std::move(menuController));

    dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_w, InputState::Down, std::make_unique<MenuSelectCommand>(true));
    dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_s, InputState::Down, std::make_unique<MenuSelectCommand>(false));
    dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_RETURN, InputState::Down, std::make_unique<MenuActivateCommand>());

    dae::InputManager::GetInstance().BindControllerCommand(0, dae::XInputManager::GetXInputValue(GamepadButton::DpadUp), InputState::Down, std::make_unique<MenuSelectCommand>(true));
    dae::InputManager::GetInstance().BindControllerCommand(0, dae::XInputManager::GetXInputValue(GamepadButton::DpadDown), InputState::Down, std::make_unique<MenuSelectCommand>(false));
    dae::InputManager::GetInstance().BindControllerCommand(0, dae::XInputManager::GetXInputValue(GamepadButton::South), InputState::Down, std::make_unique<MenuActivateCommand>());
}

void SceneManager::CreateHighscoreScene(int playerScore, GameMode mode)
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("HighScore");
    dae::SceneManager::GetInstance().SetActiveScene("HighScore");

    auto bg = GameObjectBuilder()
        .WithTexture("background.png", -2.f, 1.f)
        .SetPosition(glm::vec3(0.f, 0.f, 0.f))
        .Build();
    scene.Add(std::move(bg));

    auto font = dae::ResourceManager::GetInstance().LoadFont("Minecraft.ttf", 36);
    auto gameOverText = CreateUIText("Game Over", font, glm::vec3(200, 100, 0));
    scene.Add(std::move(gameOverText));

    auto highScores = HighscoreLoader::ReadHighScores(mode);
    std::sort(highScores.begin(), highScores.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    int numToShow = std::min(3, static_cast<int>(highScores.size()));
    for (int i = 0; i < numToShow; ++i) {
        std::string text = std::to_string(i + 1) + ". " + highScores[i].first + " - " + std::to_string(highScores[i].second);
        auto scoreText = CreateUIText(text, font, glm::vec3(200, 150 + i * 40, 0));
        scene.Add(std::move(scoreText));
    }

    std::string yourScoreText = "Your Score: " + std::to_string(playerScore);
    auto yourScore = CreateUIText(yourScoreText, font, glm::vec3(200, 150 + numToShow * 40 + 20, 0));
    scene.Add(std::move(yourScore));

    dae::InputManager::GetInstance().BindKeyboardCommand(SDLK_RETURN, InputState::Down, std::make_unique<GoToMenuCommand>());
    dae::InputManager::GetInstance().BindControllerCommand(0, dae::XInputManager::GetXInputValue(GamepadButton::South), InputState::Down, std::make_unique<GoToMenuCommand>());
}


