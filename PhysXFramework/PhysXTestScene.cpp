#include "stdafx.h"
#include "PhysXTestScene.h"
#include "CubePosColorNorm.h"

void PhysXTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };
	const PxMaterial* pDefaultMaterial{ pPhysX->createMaterial(.5f, .5f, .2f) };

	//CUBE
	const XMFLOAT3 actorDimensions{ 1.f, 1.f, 1.f };
	m_pCube = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	AddGameObject(m_pCube);

	//CUBE ACTOR
	PxRigidDynamic* pCubeActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxBoxGeometry boxGeometry{ PxBoxGeometry{ actorDimensions.x * .5f, actorDimensions.y * .5f, actorDimensions.z * .5f } };

	//Long method
	//PxShape* pCubeShape{ pPhysX->createShape(boxGeometry, *pDefaultMaterial, true) };
	//pCubeActor->attachShape(*pCubeShape);
	//m_pPhysxScene->addActor(*pCubeActor);

	//Short method
	PxRigidActorExt::createExclusiveShape(*pCubeActor, boxGeometry, *pDefaultMaterial);

	//Link CUBE with CUBE_ACTOR
	m_pCube->AttachRigidActor(pCubeActor);

	m_pCube->Translate(.0f, 10.f, .0f);
	m_pCube->RotateDegrees(30.f, 45.f, 25.f);

	//GROUND PLANE
	const auto pGroundActor{ pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{ .0f, .0f, 1.f } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);
}

void PhysXTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pCube->Translate(0.f, 10.f, 0.f);
	}
}

void PhysXTestScene::Draw() const
{
}

void PhysXTestScene::OnSceneActivated()
{
}

void PhysXTestScene::OnSceneDeactivated()
{
}
