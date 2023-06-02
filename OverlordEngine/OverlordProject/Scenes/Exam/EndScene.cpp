#include "stdafx.h"
#include "EndScene.h"
#include "Prefabs/Exam/UI/Buttons/UIButton.h"
#include "Prefabs/Exam/UI/UIManager.h"
#include "Exam/ExamAssets.h"

void EndScene::Initialize()
{
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;

	// Set-up a fixed camera
	auto pCameraObject{ new FixedCamera{} };
	AddChild(pCameraObject);
	SetActiveCamera(pCameraObject->GetComponent<CameraComponent>());

	UIManager* pUIManager{ new UIManager{} };
	AddChild(pUIManager);

	GameObject* pBackground{ new GameObject{} };
	pBackground->AddComponent(new SpriteComponent{ ExamAssets::MainMenuBackground });
	AddChild(pBackground);

	// Create a button
	auto pFont{ ContentManager::Load<SpriteFont>(ExamAssets::Font) };

	XMFLOAT2 buttonPos{ m_SceneContext.windowWidth * .05f, m_SceneContext.windowHeight * .3f };

	auto pButtonOne{ new UIButton{ pFont, L"Restart", buttonPos } };
	buttonPos.y += 75.f;
	auto pButtonTwo{ new UIButton{ pFont, L"Main Menu", { buttonPos.x, buttonPos.y } } };
	buttonPos.y += 75.f;
	auto pButtonThree{ new UIButton{ pFont, L"Exit Game", { buttonPos.x, buttonPos.y } } };

	pButtonOne->SetOnClickFunction(std::bind_front(&EndScene::MainMenu, this));
	pButtonTwo->SetOnClickFunction(std::bind_front(&EndScene::RestartGame, this));
	pButtonThree->SetOnClickFunction(std::bind_front(&EndScene::ExitGame, this));

	pButtonOne->SetNavigationButtons(pButtonThree, pButtonTwo, nullptr, nullptr);
	pButtonTwo->SetNavigationButtons(pButtonOne, pButtonThree, nullptr, nullptr);
	pButtonThree->SetNavigationButtons(pButtonTwo, pButtonOne, nullptr, nullptr);

	AddChild(pButtonOne);
	AddChild(pButtonTwo);
	AddChild(pButtonThree);

	pUIManager->AddButton(pButtonOne);
	pUIManager->AddButton(pButtonTwo);
	pUIManager->AddButton(pButtonThree);

	pUIManager->SetFirstSelectedButton(pButtonOne);

	pUIManager->EnableInput();
}

void EndScene::MainMenu()
{
	SceneManager::Get()->SetActiveGameScene(L"MainMenuScene");
}

void EndScene::RestartGame()
{
	SceneManager::Get()->SetActiveGameScene(L"LevelScene");
}

void EndScene::ExitGame()
{
	PostQuitMessage(0);
}
