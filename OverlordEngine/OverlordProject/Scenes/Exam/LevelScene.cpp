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
		SetupPlayer(m_PlayerSpawnPoints[i].x, m_PlayerSpawnPoints[i].y);
	}
}

void LevelScene::SetupPlayer(int xPos, int yPos) const
{
	auto* pPlayer{ new Player{ L"Meshes/Exam/Player.ovm" } };

	pPlayer->SetScale(.01f, .01f);
	pPlayer->SetPosition(xPos, yPos);
	pPlayer->SetSpawnPoint(xPos, yPos);
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
