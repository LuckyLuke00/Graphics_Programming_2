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

	for (auto& pObject : toAdd)
	{
		if (!pObject.first) continue;

		if (pObject.second)
		{
			Player* pPlayer{ dynamic_cast<Player*>(pObject.first) };
			if (pPlayer) m_pGridMap->AddPlayer(pPlayer);
			else m_pGridMap->AddGridObject(pObject.first);
		}

		m_pGridMap->AddChild(pObject.first);
	}

	GridObject::GetObjectsToAdd().clear();

	const auto& toDestroy{ GridObject::GetObjectsToDestroy() };

	for (auto* pObject : toDestroy)
	{
		if (!pObject) continue;

		Player* pPlayer{ dynamic_cast<Player*>(pObject) };
		if (pPlayer) m_pGridMap->RemovePlayer(pPlayer);
		else m_pGridMap->RemoveGridObject(pObject);

		m_pGridMap->RemoveChild(pObject);
	}

	GridObject::GetObjectsToDestroy().clear();
}

void LevelScene::OnSceneActivated()
{
	for (int i{ 0 }; i < m_MaxPlayers; ++i)
	{
		SetupPlayer(i);
	}
}

void LevelScene::SetupPlayer(int playerIndex) const
{
	if (playerIndex < 0 || playerIndex >= m_MaxPlayers) return;

	// Calculate the corner of the grid where the player should spawn
	// If playerindex is 0, spawn in the bottom left corner, bottom left is (1, 1)
	// If playerindex is 1, spawn in the top right corner
	// If playerindex is 2, spawn in the bottom right corner
	// If playerindex is 3, spawn in the top left corner

	// Use m_pGridMap->GetCols and m_pGridMap->GetRows to calculate the spawn point
	// PlayerIndex goes from 0 to 3
	const int x{ playerIndex % 2 == 0 ? 1 : m_pGridMap->GetRows() - 2 };
	const int y{ playerIndex < 2 ? 1 : m_pGridMap->GetCols() - 2 };

	auto* pPlayer{ new Player{ L"Meshes/Exam/Player.ovm" } };

	pPlayer->SetScale(.01f, .01f);
	pPlayer->SetPosition(x, y);
	pPlayer->SetSpawnPoint(x, y);
	pPlayer->SetDimensions(1, 1);
	pPlayer->OffsetPosition(.0f, -.5f, .0f);
	pPlayer->MarkForAdd();
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
