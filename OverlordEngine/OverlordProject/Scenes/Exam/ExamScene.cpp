#include "stdafx.h"
#include "ExamScene.h"

void ExamScene::Initialize()
{
	//auto& pPhysX{ PxGetPhysics() };
	//auto pDefaultMaterial{ pPhysX.createMaterial(.5f, .5f, .5f) };

	// Set-up a fixed camera
	auto pCameraObject = new FixedCamera{};
	AddChild(pCameraObject);
	SetActiveCamera(pCameraObject->GetComponent<CameraComponent>());

	LoadSprites();
	LoadMenu(Menus::MainMenu);
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
