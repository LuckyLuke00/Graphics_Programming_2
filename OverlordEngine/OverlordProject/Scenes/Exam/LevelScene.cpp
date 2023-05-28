#include "stdafx.h"
#include "LevelScene.h"
#include <Prefabs/Exam/Level/GridMap.h>
#include <Components/GridMovementComponent.h>
#include <Materials/DiffuseMaterial.h>
#include <Prefabs/Exam/Player/Player.h>

void LevelScene::Initialize()
{
	GetSceneSettings().drawGrid = false;
	GetSceneSettings().drawPhysXDebug = false;

	SetUpCamera();
	CreateGroundPlane();

	auto* pGridMap{ new GridMap{ 19, 13 } };
	AddChild(pGridMap);
}

void LevelScene::OnSceneActivated()
{
	auto* pPlayer{ new Player{ L"Meshes/Exam/UnitBox.ovm", L"" } };
	pPlayer->SetPosition(1, 1);
	pPlayer->SetDimensions(1, 1);

	AddChild(pPlayer);
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
