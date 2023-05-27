#include "stdafx.h"
#include "LevelScene.h"
#include <Prefabs/Exam/Level/GridMap.h>
#include <Components/GridMovementComponent.h>
#include <Materials/DiffuseMaterial.h>
#include <Prefabs/Exam/Player/Player.h>

void LevelScene::Initialize()
{
	auto pCameraObject{ new FixedCamera{} };
	AddChild(pCameraObject);
	SetActiveCamera(pCameraObject->GetComponent<CameraComponent>());

	pCameraObject->GetTransform()->Translate(10, 20, 1);
	pCameraObject->GetTransform()->Rotate(75, 0, 0);

	auto* pGridMap{ new GridMap{ 19, 13 } };
	AddChild(pGridMap);
}

void LevelScene::OnSceneActivated()
{
	auto* pPlayer{ new Player{ L"Meshes/UnitBox.ovm", L"" } };
	pPlayer->SetScale(.5f, .5f);
	pPlayer->SetPosition(1, 1);
	pPlayer->SetDimensions(1, 1);

	AddChild(pPlayer);
}
