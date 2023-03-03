#include "stdafx.h"
#include "W2_AssignmentScene.h"
#include "ContentManager.h"
#include "CubePosColorNorm.h"
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

	// Use a PxD6Joint to lock the sphere’s movement in the X / Y plane
	const auto pPlayerJoint{ PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pPlayerSphereActor, PxTransform{ PxIdentity }) };
	pPlayerJoint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	pPlayerJoint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	pPlayerJoint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	pPlayerJoint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
	pPlayerJoint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);

	const auto pRedSphereActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidActorExt::createExclusiveShape(*pRedSphereActor, PxSphereGeometry{ m_SmallSphereRadius }, *pDefaultMaterial);
	m_pRedSphere->AttachRigidActor(pRedSphereActor);
	m_pRedSphere->Translate(1.f, 21.0f, .0f);

	// Boxes
	m_pBlueBox = new CubePosColorNorm(m_BoxSize, m_BoxSize, m_BoxSize, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pBlueBox);

	m_pRedBox = new CubePosColorNorm(m_BoxSize, m_BoxSize, m_BoxSize, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pRedBox);

	const auto pBlueBoxActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidActorExt::createExclusiveShape(*pBlueBoxActor, PxBoxGeometry{ m_BoxSize * .5f, m_BoxSize * .5f, m_BoxSize * .5f }, *pDefaultMaterial);
	m_pBlueBox->AttachRigidActor(pBlueBoxActor);
	m_pBlueBox->Translate(-2.5f, 5.f, .0f);
	pBlueBoxActor->setMass(m_BoxMass);

	const auto pRedBoxActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidActorExt::createExclusiveShape(*pRedBoxActor, PxBoxGeometry{ m_BoxSize * .5f, m_BoxSize * .5f, m_BoxSize * .5f }, *pDefaultMaterial);
	m_pRedBox->AttachRigidActor(pRedBoxActor);
	m_pRedBox->Translate(2.5f, 5.f, .0f);
	pRedBoxActor->setMass(m_BoxMass);
}

void W2_AssignmentScene::Update()
{
	constexpr float speed{ 25.f };
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_UP))
	{
		m_pPlayerSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ speed, .0f, .0f });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_DOWN))
	{
		m_pPlayerSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ -speed, .0f, .0f });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pPlayerSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ .0f, .0f, speed });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pPlayerSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ .0f, .0f, -speed });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, VK_SPACE))
	{
		m_pPlayerSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ .0f, speed * 15.f, .0f });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		ResetScene();
	}
}

void W2_AssignmentScene::Draw() const
{
}

void W2_AssignmentScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"\t[INPUT > Reset='R' | Move_Sphere='Arrow Keys' | Jump_Sphere='Space']");
}

void W2_AssignmentScene::OnSceneDeactivated()
{
}

void W2_AssignmentScene::ResetScene()
{
	// Reset spheres
	m_pBlueSphere->Translate(-1.f, 21.f, .0f);
	m_pPlayerSphere->Translate(.0f, 5.f, .0f);
	m_pRedSphere->Translate(1.f, 21.0f, .0f);

	// Set velocities and angular velocities to zero
	m_pBlueSphere->GetRigidActor()->is<PxRigidDynamic>()->setLinearVelocity({ .0f, .0f, .0f });
	m_pBlueSphere->GetRigidActor()->is<PxRigidDynamic>()->setAngularVelocity({ .0f, .0f, .0f });
	
	m_pPlayerSphere->GetRigidActor()->is<PxRigidDynamic>()->setLinearVelocity({ .0f, .0f, .0f });
	m_pPlayerSphere->GetRigidActor()->is<PxRigidDynamic>()->setAngularVelocity({ .0f, .0f, .0f });
	
	m_pRedSphere->GetRigidActor()->is<PxRigidDynamic>()->setLinearVelocity({ .0f, .0f, .0f });
	m_pRedSphere->GetRigidActor()->is<PxRigidDynamic>()->setAngularVelocity({ .0f, .0f, .0f });

	// Reset boxes
	m_pBlueBox->Translate(-2.5f, 5.f, .0f);
	m_pRedBox->Translate(2.5f, 5.f, .0f);

	// Reset the rotation of the boxes
	m_pBlueBox->Rotate(.0f, .0f, .0f);
	m_pRedBox->Rotate(.0f, .0f, .0f);
}
