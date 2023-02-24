#include "stdafx.h"
#include "W1_AssignmentScene.h"
#include "CubePosColorNorm.h"
#include <vector>

void W1_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	//CAMERA
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ .0f, 10.f, -30.f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ .0f, -.265f, 1.f });

	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };
	
	// Materials
	const PxMaterial* pFloorMaterial{ pPhysX->createMaterial(.6f, .6f, .2f) };
	const PxMaterial* pWallMaterial{ pPhysX->createMaterial(.7f, .7f, .4f) };
	const PxMaterial* pSphereMaterial{ pPhysX->createMaterial(.9f, .9f, .8f) };

	//FLOOR
	m_pFloor = new CubePosColorNorm(m_FloorSize, .0f, m_FloorSize, XMFLOAT4{ Colors::LightGray });
	PxRigidStatic* pFloorActor{ pPhysX->createRigidStatic( PxTransform{ PxIdentity }) };
	PxBoxGeometry floorGeometry{ PxBoxGeometry{ m_FloorSize * .5f, .0f, m_FloorSize * .5f } };
	PxRigidActorExt::createExclusiveShape(*pFloorActor, floorGeometry, *pFloorMaterial);
	m_pFloor->AttachRigidActor(pFloorActor);
	AddGameObject(m_pFloor);

	//WALLS
	CreateWall(100, 10, .25f, pWallMaterial, pPhysX);

}

void W1_AssignmentScene::Update()
{
}

void W1_AssignmentScene::Draw() const
{
}

void W1_AssignmentScene::OnSceneActivated()
{
}

void W1_AssignmentScene::OnSceneDeactivated()
{
}

void W1_AssignmentScene::CreateWall(const int boxes, const int verticalBoxes, const float separation, const PxMaterial* pMaterial, PxPhysics* pPhysX)
{
	std::vector<GameObject*> wall;
	wall.reserve(boxes);

	const float xStart{ -((boxes / verticalBoxes - 1) * (separation + m_BoxSize)) * .5f };

	for (int i{ 0 }; i < boxes; ++i)
	{
		wall.emplace_back(new CubePosColorNorm(m_BoxSize, m_BoxSize, m_BoxSize) );
		PxRigidDynamic* pBoxActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
		PxBoxGeometry boxGeometry{ PxBoxGeometry{ m_BoxSize * .5f, m_BoxSize * .5f, m_BoxSize * .5f } };
		PxRigidActorExt::createExclusiveShape(*pBoxActor, boxGeometry, *pMaterial);
		wall.back()->AttachRigidActor(pBoxActor);
		AddGameObject(wall.back());

		// Calculate x position
		const float x{ xStart + (i % (boxes / verticalBoxes)) * (separation + m_BoxSize) };

		// Calculate y position
		const float y{ m_BoxSize * .5f + (i / (boxes / verticalBoxes)) * m_BoxSize };

		// Calculate z position (at the back of the floor)
		const float back{ m_FloorSize * .5f - m_BoxSize * .5f };
		// A quarter of the way back
		const float z{ back - back * .5f };

		wall.back()->Translate(x, y, z);

		// Give them a small random rotation around the y-axis
		wall.back()->RotateDegrees(.0f, static_cast<float>(rand() % 25), .0f);
	}
}
