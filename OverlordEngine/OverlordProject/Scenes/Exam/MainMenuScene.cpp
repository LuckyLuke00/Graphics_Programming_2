#include "stdafx.h"
#include "MainMenuScene.h"
#include "Prefabs/Exam/UI/Buttons/UIButton.h"
#include "Prefabs/Exam/UI/UIManager.h"
#include "Exam/ExamAssets.h"

void MainMenuScene::Initialize()
{
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;

	// Set-up a fixed camera
	auto pCameraObject{ new FixedCamera{} };
	AddChild(pCameraObject);
	SetActiveCamera(pCameraObject->GetComponent<CameraComponent>());

	m_pUIManager = new UIManager{};
	AddChild(m_pUIManager);

	GameObject* pBackground{ new GameObject{} };
	pBackground->AddComponent(new SpriteComponent{ ExamAssets::MainMenuBackground });
	AddChild(pBackground);

	// Create a button
	auto pFont{ ContentManager::Load<SpriteFont>(ExamAssets::Font) };

	const XMFLOAT2 buttonPos{ m_SceneContext.windowWidth * .05f, m_SceneContext.windowHeight * .3f };

	auto pButtonOne{ new UIButton{ pFont, L"Start Game", buttonPos } };
	auto pButtonTwo{ new UIButton{ pFont, L"Quit", { buttonPos.x, buttonPos.y + 35.f } } };

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

void MainMenuScene::ExitGame()
{
	PostQuitMessage(0);
}

void MainMenuScene::StartGame()
{
	SceneManager::Get()->SetActiveGameScene(L"LevelScene");
}
