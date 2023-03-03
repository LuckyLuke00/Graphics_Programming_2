#include "stdafx.h"
#include "W2_AssignmentScene.h"
#include "ContentManager.h"
#include "Logger.h"
#include "MeshObject.h"
#include "PhysxManager.h"
#include "SpherePosColorNorm.h"


void W2_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };
	auto pDefaultMaterial{ pPhysX->createMaterial(.5f, .5f, .2f) };

	// Ground Plane
	const auto pGroundActor{ pPhysX->createRigidStatic(physx::PxTransform{ { PxPiDivTwo, { 0, 0, 1 } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	// Triangle Level
	m_pLevelTriangle = new MeshObject{ L"Resources/Meshes/Level.ovm" };
	AddGameObject(m_pLevelTriangle);

	const auto pTriangleMesh{ ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt") };
	const auto pTriangleActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	m_pLevelTriangle->AttachRigidActor(pTriangleActor);

	// Spheres
	m_pBlueSphere = new SpherePosColorNorm(m_SmallSphereRadius, 10, 10, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pBlueSphere);

	m_pPlayerSphere = new SpherePosColorNorm(m_PlayerSphereRadius, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pPlayerSphere);

	m_pRedSphere = new SpherePosColorNorm(m_SmallSphereRadius, 10, 10, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pRedSphere);

	const auto pBlueSphereActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidActorExt::createExclusiveShape(*pBlueSphereActor, PxSphereGeometry{ m_SmallSphereRadius }, *pDefaultMaterial);
	m_pBlueSphere->AttachRigidActor(pBlueSphereActor);
	m_pBlueSphere->Translate(-1.f, 21.f, .0f);

	const auto pPlayerSphereActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidActorExt::createExclusiveShape(*pPlayerSphereActor, PxSphereGeometry{ m_PlayerSphereRadius }, *pDefaultMaterial);
	m_pPlayerSphere->AttachRigidActor(pPlayerSphereActor);
	m_pPlayerSphere->Translate(.0f, 5.f, .0f);

	const auto pRedSphereActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidActorExt::createExclusiveShape(*pRedSphereActor, PxSphereGeometry{ m_SmallSphereRadius }, *pDefaultMaterial);
	m_pRedSphere->AttachRigidActor(pRedSphereActor);
	m_pRedSphere->Translate(1.f, 21.0f, .0f);

}

void W2_AssignmentScene::Update()
{
}

void W2_AssignmentScene::Draw() const
{
}

void W2_AssignmentScene::OnSceneActivated()
{
}

void W2_AssignmentScene::OnSceneDeactivated()
{
}
