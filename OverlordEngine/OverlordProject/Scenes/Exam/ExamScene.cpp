#include "stdafx.h"
#include "ExamScene.h"
#include "Prefabs/Exam/UI/Buttons/UIButton.h"
#include "Prefabs/Exam/UI/UIManager.h"

void ExamScene::Initialize()
{
	//auto& pPhysX{ PxGetPhysics() };
	//auto pDefaultMaterial{ pPhysX.createMaterial(.5f, .5f, .5f) };

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

	pButtonOne->SetOnClickFunction(std::bind_front(&ExamScene::StartGame, this));
	pButtonTwo->SetOnClickFunction(std::bind_front(&ExamScene::ExitGame, this));

	AddChild(pButtonOne);
	AddChild(pButtonTwo);

	m_pUIManager->AddButton(pButtonOne);
	m_pUIManager->AddButton(pButtonTwo);

	// Enable input
	m_pUIManager->EnableInput();
}

void ExamScene::LoadMenu(const Menus& menu)
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

void ExamScene::LoadSprites()
{
	// Load all menu sprites
	m_pMenuBackgroundSprites[Menus::MainMenu] = new GameObject();
	m_pMenuBackgroundSprites[Menus::MainMenu]->AddComponent(new SpriteComponent(L"Textures/Menus/MainMenu_Background.png"));
}

void ExamScene::ExitGame()
{
	std::cout << "Exit Game" << std::endl;
}

void ExamScene::StartGame()
{
	std::cout << "Start Game" << std::endl;
}
