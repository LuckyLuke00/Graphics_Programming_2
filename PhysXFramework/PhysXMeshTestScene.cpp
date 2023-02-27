#include "stdafx.h"
#include "PhysXMeshTestScene.h"

#include "PhysxManager.h"
#include "MeshObject.h"
#include "ContentManager.h"

void PhysXMeshTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysx{ PhysxManager::GetInstance()->GetPhysics() };
	auto pDefaultMaterial{ pPhysx->createMaterial(.5f, .5f, .2f) };

	// Ground Plane
	const auto pGroundActor{ pPhysx->createRigidStatic(physx::PxTransform{ { PxPiDivTwo, { 0, 0, 1 } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	// Convex Chair
	m_pChairConvex = new MeshObject{ L"Resources/Meshes/Chair.ovm" };
	AddGameObject(m_pChairConvex);

	const auto pConvexMesh{ ContentManager::GetInstance()->Load<PxConvexMesh>(L"Resources/Meshes/Chair.ovpc") };
	const auto pConvexActor{ pPhysx->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidActorExt::createExclusiveShape(*pConvexActor, PxConvexMeshGeometry{ pConvexMesh }, *pDefaultMaterial);

	PxVec3 massLocalPose{ .0f, 4.f, .0f };
	PxRigidBodyExt::updateMassAndInertia(*pConvexActor, 10.f, &massLocalPose);

	m_pChairConvex->AttachRigidActor(pConvexActor);
	m_pChairConvex->Translate(.0f, 12.f, .0f);

	// Triangle Chair
	m_pChairTriangle = new MeshObject{ L"Resources/Meshes/Chair.ovm" };
	AddGameObject(m_pChairTriangle);

	const auto pTriangleMesh{ ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Chair.ovpt") };
	const auto pTriangleActor{ pPhysx->createRigidDynamic(PxTransform{ PxIdentity }) };
	pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	m_pChairTriangle->AttachRigidActor(pTriangleActor);

	m_pChairTriangle->Translate(10.f, .0f, .0f);
	m_KinematicPosition = { 10.f, .0f, .0f };

}

void PhysXMeshTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pChairConvex->Translate(.0f, 12.f, .0f);
		m_pChairConvex->Rotate(.0f, .0f, .0f);

		m_pChairTriangle->Translate(10.f, .0f, .0f);
		m_KinematicPosition = { 10.f, .0f, .0f };
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_KinematicPosition.x -= 5.f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{ m_KinematicPosition });
		//m_pChairTriangle->GetRigidActor()->setGlobalPose(PxTransform{ m_KinematicPosition }); //// Teleporting
	}
}

void PhysXMeshTestScene::Draw() const
{
}

void PhysXMeshTestScene::OnSceneActivated()
{
}

void PhysXMeshTestScene::OnSceneDeactivated()
{
}
