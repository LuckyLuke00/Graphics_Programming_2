#include "stdafx.h"
#include "LevelScene.h"
#include <Components/GridMovementComponent.h>
#include <Materials/DiffuseMaterial.h>
#include <Prefabs/Exam/Player/Player.h>

void LevelScene::Initialize()
{
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;

	SetUpCamera();
	CreateGroundPlane();

	m_pGridMap = AddChild(new GridMap{ 19, 13 });
}

void LevelScene::Update()
{
	const auto& toAdd{ GridObject::GetObjectsToAdd() };

	for (auto* pObject : toAdd)
	{
		m_pGridMap->AddChild(pObject);
	}

	GridObject::GetObjectsToAdd().clear();

	const auto& toDestroy{ GridObject::GetObjectsToDestroy() };

	for (auto* pObject : toDestroy)
	{
		m_pGridMap->RemoveChild(pObject);
	}

	GridObject::GetObjectsToDestroy().clear();
}

void LevelScene::OnSceneActivated()
{
	auto* pPlayer{ new Player{ L"Meshes/Exam/Player.ovm" } };
	pPlayer->SetScale(.01f, .01f);
	pPlayer->SetPosition(1, 1);
	pPlayer->SetDimensions(1, 1);
	pPlayer->OffsetPosition(.0f, -.5f, .0f);
	m_pGridMap->AddPlayer(pPlayer);
	AddChild(pPlayer);

	auto* pPlayer2{ new Player{ L"Meshes/Exam/Player.ovm" } };
	pPlayer2->SetScale(.01f, .01f);
	pPlayer2->SetPosition(17, 11);
	pPlayer2->SetDimensions(1, 1);
	pPlayer2->OffsetPosition(.0f, -.5f, .0f);
	AddChild(pPlayer2);
	m_pGridMap->AddPlayer(pPlayer2);

	auto* pPlayer3{ new Player{ L"Meshes/Exam/Player.ovm" } };
	pPlayer3->SetScale(.01f, .01f);
	pPlayer3->SetPosition(17, 1);
	pPlayer3->SetDimensions(1, 1);
	pPlayer3->OffsetPosition(.0f, -.5f, .0f);
	AddChild(pPlayer3);
	m_pGridMap->AddPlayer(pPlayer3);

	auto* pPlayer4{ new Player{ L"Meshes/Exam/Player.ovm" } };
	pPlayer4->SetScale(.01f, .01f);
	pPlayer4->SetPosition(1, 11);
	pPlayer4->SetDimensions(1, 1);
	pPlayer4->OffsetPosition(.0f, -.5f, .0f);
	AddChild(pPlayer4);
	m_pGridMap->AddPlayer(pPlayer4);
}

void LevelScene::SetUpCamera()
{
	auto pCameraObject{ new FixedCamera{} };
	AddChild(pCameraObject);
	SetActiveCamera(pCameraObject->GetComponent<CameraComponent>());

	pCameraObject->GetTransform()->Translate(10, 20, 1);
	pCameraObject->GetTransform()->Rotate(75, 0, 0);
}

void LevelScene::CreateGroundPlane()
{
	auto* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	pMaterial->SetDiffuseTexture(L"Textures/Exam/dark_oak_planks.png");

	for (int i{ 0 }; i < 4; ++i)
	{
		for (int j{ 0 }; j < 4; ++j)
		{
			auto* pGround{ new GameObject{} };
			auto* pModel{ pGround->AddComponent(new ModelComponent{ L"Meshes/UnitPlane.ovm", false }) };
			pModel->SetMaterial(pMaterial);
			pGround->GetTransform()->Translate(-5.f + 10.f * static_cast<float>(i), -.1f, 10.f * static_cast<float>(j));
			AddChild(pGround);
		}
	}
}
