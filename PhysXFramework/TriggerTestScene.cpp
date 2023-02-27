#include "stdafx.h"
#include "TriggerTestScene.h"
#include "SpherePosColorNorm.h"

#include "PhysxManager.h"
#include "Logger.h"

void TriggerTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };
	auto pDefaultMaterial{ pPhysX->createMaterial(.5f, .5f, .2f) };

	// Ground Plane
	const auto pGroundActor{ pPhysX->createRigidStatic(physx::PxTransform{ { PxPiDivTwo, { 0, 0, 1 } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	// Sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere);

	const auto pSphereActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(.0f, 2.f, .0f);

	//TRIGGER LEFT
	m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ { -8.f, .5f, .0f } });
	auto pShape{ PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ .5f, .5f, .5f }, *pDefaultMaterial) };

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerLeft);

	//TRIGGER RIGHT
	m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ { 8.f, .5f, .0f } });
	pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ .5f, .5f, .5f }, *pDefaultMaterial);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerRight);
}

void TriggerTestScene::Update()
{
	constexpr float force{ 10.f };
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, force });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, -force });
	}

	// Trigger Check
	if (m_IsTriggerLeft)
	{
		m_IsTriggerLeft = false;
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ .0f, 10.f, .0f }, PxForceMode::eIMPULSE);
	}
}

void TriggerTestScene::Draw() const
{
}

void TriggerTestScene::OnSceneActivated()
{
}

void TriggerTestScene::OnSceneDeactivated()
{
}

void TriggerTestScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (size_t i{ 0 }; i < count; ++i)
	{
		// Ignore deleted shapes
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		const PxTriggerPair& pair{ pairs[i] };
		
		if (pair.triggerActor == m_pTriggerLeft)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) // ENTERED
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND trigger box LEFT");
				m_IsTriggerLeft = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) // LEFT
			{
				Logger::GetInstance()->LogInfo(L"Sphere LOST trigger box LEFT");
			}
		}

		if (pair.triggerActor == m_pTriggerRight)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) // ENTERED
			{
				m_IsTriggerRight = true;
				Logger::GetInstance()->LogInfo(L"Sphere FOUND trigger box RIGHT");
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST) // LEFT
			{
				Logger::GetInstance()->LogInfo(L"Sphere LOST trigger box RIGHT");
			}
		}
	}
}
