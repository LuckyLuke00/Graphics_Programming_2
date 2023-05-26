#include "stdafx.h"
#include "MainMenuScene.h"
#include "Prefabs/Exam/UI/Buttons/UIButton.h"
#include "Prefabs/Exam/UI/UIManager.h"

void MainMenuScene::Initialize()
{
	// Set-up a fixed camera
	auto pCameraObject = new FixedCamera{};
	AddChild(pCameraObject);
	SetActiveCamera(pCameraObject->GetComponent<CameraComponent>());

	m_pUIManager = new UIManager{};
	AddChild(m_pUIManager);

	LoadSprites();
	LoadMenu(Menus::MainMenu);

	// Create a button
	auto pFont{ ContentManager::Load<SpriteFont>(L"SpriteFonts/Consolas_32.fnt") };
	auto pButtonOne{ new UIButton{ pFont, L"Start Game", { 100, 100 } } };
	auto pButtonTwo{ new UIButton{ pFont, L"Exit Game", { 100, 200 } } };

	pButtonOne->SetOnClickFunction(std::bind_front(&MainMenuScene::StartGame, this));
	pButtonTwo->SetOnClickFunction(std::bind_front(&MainMenuScene::ExitGame, this));

	pButtonOne->SetNavigationButtons(pButtonTwo, pButtonTwo, nullptr, nullptr);
	pButtonTwo->SetNavigationButtons(pButtonOne, pButtonOne, nullptr, nullptr);

	AddChild(pButtonOne);
	AddChild(pButtonTwo);

	m_pUIManager->AddButton(pButtonOne);
	m_pUIManager->AddButton(pButtonTwo);

	m_pUIManager->SetFirstSelectedButton(pButtonOne);

	m_pUIManager->EnableInput();
}

void MainMenuScene::LoadMenu(const Menus& menu)
{
	// If the menu is already loaded, return
	if (m_CurrentMenu == menu) return;

	// Remove the current menu from the scene
	if (m_CurrentMenu != Menus::None)
	{
		RemoveChild(m_pMenuBackgroundSprites[m_CurrentMenu]);
	}

	// Add the new menu to the scene
	m_CurrentMenu = menu;
	if (m_CurrentMenu != Menus::None)
	{
		AddChild(m_pMenuBackgroundSprites[m_CurrentMenu]);
	}
}

void MainMenuScene::LoadSprites()
{
	// Load all menu sprites
	m_pMenuBackgroundSprites[Menus::MainMenu] = new GameObject();
	m_pMenuBackgroundSprites[Menus::MainMenu]->AddComponent(new SpriteComponent(L"Textures/Menus/MainMenu_Background.png"));
}

void MainMenuScene::ExitGame()
{
	std::cout << "Exit Game\n";

	PostQuitMessage(0);
}

void MainMenuScene::StartGame()
{
	std::cout << "Start Game\n";

	SceneManager::Get()->SetActiveGameScene(L"LevelScene");
}
