#include "stdafx.h"
#include "LevelScene.h"
#include "Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "Prefabs/Exam/Player/Player.h"
#include "Exam/ExamAssets.h"
#include "Prefabs/Exam/Level/GridMap.h"
#include "Prefabs/Exam/UI/CountdownTimer.h"

void LevelScene::TogglePause()
{
	m_Paused = !m_Paused;

	if (m_Paused)
	{
		m_pCountdownTimer->PauseTimer();
	}
	else
	{
		m_pCountdownTimer->ResumeTimer();
	}
}

void LevelScene::Initialize()
{
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;

	m_SceneContext.pLights->SetDirectionalLight({ .0f, 50.f, .0f }, { .2f, -.7f, .4f });

	SetUpCamera();
	CreateGroundPlane();
}

void LevelScene::Update()
{
	AddGridObjects();
	RemoveGridObjects();

	if (HasGameEnded())
	{
		m_pGridMap->Reset();
		m_pCountdownTimer->StopTimer();

		SceneManager::Get()->SetActiveGameScene(L"MainMenuScene");
	}
}

void LevelScene::OnSceneActivated()
{
	m_pGridMap = AddChild(new GridMap{ 19, 13 });

	// Middle of the screen at the top
	const XMFLOAT2 timerTextPos{ m_SceneContext.windowWidth * .5f, m_SceneContext.windowHeight * .05f };
	m_pCountdownTimer = new CountdownTimer{ ExamAssets::Font, timerTextPos };
	m_pCountdownTimer->SetCountdownTime(180.f);
	m_pCountdownTimer->StartTimer();
	AddChild(m_pCountdownTimer);

	for (int i{ 0 }; i < m_MaxPlayers; ++i)
	{
		SetupPlayer(i);
	}
}

void LevelScene::SetupPlayer(int playerIndex) const
{
	if (playerIndex < 0 || playerIndex >= m_MaxPlayers) return;

	// Calculate the corner of the grid where the player should spawn
	const int x{ playerIndex % 2 == 0 ? 1 : m_pGridMap->GetRows() - 2 };
	const int y{ playerIndex < 2 ? 1 : m_pGridMap->GetCols() - 2 };

	auto* pPlayer{ new Player{ ExamAssets::PlayerMesh } };

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
	auto* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>() };
	pMaterial->SetDiffuseTexture(ExamAssets::BackgroundTexture);

	for (int i{ 0 }; i < 4; ++i)
	{
		for (int j{ 0 }; j < 4; ++j)
		{
			auto* pGround{ new GameObject{} };
			auto* pModel{ pGround->AddComponent(new ModelComponent{ L"Meshes/UnitPlane.ovm", false }) };
			pModel->SetMaterial(pMaterial);
			pGround->GetTransform()->Translate(-5.f + 10.f * static_cast<float>(i), -.5f, 10.f * static_cast<float>(j));
			AddChild(pGround);
		}
	}
}

void LevelScene::RemoveGridObjects()
{
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

void LevelScene::AddGridObjects()
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
}

bool LevelScene::HasGameEnded() const
{
	// Check if the timer has run out
	if (m_pCountdownTimer->HasCountdownEnded()) return true;

	// Check if the Deadplayers vector is greater than or equal to the amount of players
	if (m_pGridMap->GetDeadPlayers().size() >= m_MaxPlayers) return true;

	return false;
}
