#include "stdafx.h"
#include "FrictionTestScene.h"
#include "CubePosColorNorm.h"
#include "Logger.h"

void FrictionTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };
	const PxMaterial* pFrictionlessMaterial{ pPhysX->createMaterial(.0f, .0f, .2f) };

	//CAMERA
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ .0f, 10.f, -30.f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ .0f, -.265f, 1.f });

	//GROUND PLANE
	const auto pGroundActor{ pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{ .0f, .0f, 1.f } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pFrictionlessMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//BOXES
	const float xStart{ -((m_pBoxes.size() - 1) * m_Separation) * .5f };

	const XMFLOAT3 boxDimensions{ 1.f, 1.f, 1.f };
	const XMFLOAT3 rampDimensions{ 6.f, 1.f, 2.f };

	for (int i{ 0 }; i < m_pBoxes.size(); ++i)
	{
		m_pBoxes[i] = new CubePosColorNorm(1.f, 1.f, 1.f);
		AddGameObject(m_pBoxes[i]);

		m_pRamps[i] = new CubePosColorNorm(6.f, 1.f, 2.f);
		AddGameObject(m_pRamps[i]);

		m_pBoxes[i]->Translate(xStart + (static_cast<float>(i) * m_Separation) - 1.f, 7.f, .0f);
		m_pRamps[i]->Translate(xStart + (static_cast<float>(i) * m_Separation), 2.f, .0f);

		m_pBoxes[i]->RotateDegrees(0.f, 0.f, -30.f);
		m_pRamps[i]->RotateDegrees(0.f, 0.f, -30.f);

		PxRigidDynamic* pBoxActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
		PxRigidStatic* pRampActor{ pPhysX->createRigidStatic(PxTransform{ PxIdentity }) };

		PxBoxGeometry boxGeometry{ boxDimensions.x * .5f, boxDimensions.y * .5f, boxDimensions.z * .5f };
		PxBoxGeometry rampGeometry{ rampDimensions.x * .5f, rampDimensions.y * .5f, rampDimensions.z * .5f };

		const float friction{ i / static_cast<float>(m_pBoxes.size() - 1) };
		const PxMaterial* pFrictionMaterial{ pPhysX->createMaterial(friction, friction, .2f) };

		PxRigidActorExt::createExclusiveShape(*pBoxActor, boxGeometry, *pFrictionMaterial);
		PxRigidActorExt::createExclusiveShape(*pRampActor, rampGeometry, *pFrictionlessMaterial);

		m_pBoxes[i]->AttachRigidActor(pBoxActor);
		m_pRamps[i]->AttachRigidActor(pRampActor);
	}
}

void FrictionTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		//BALLS
		const float xStart{ -((m_pBoxes.size() - 1) * m_Separation) * .5f };

		for (int i{ 0 }; i < m_pBoxes.size(); ++i)
		{
			m_pBoxes[i]->Translate(xStart + (static_cast<float>(i) * m_Separation) - 1.f, 7.f, .0f);
			m_pBoxes[i]->RotateDegrees(0.f, 0.f, -30.f);
		}

		//CAMERA
		m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ .0f, 10.f, -30.f });
		m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ .0f, -.265f, 1.f });
	}
}

void FrictionTestScene::Draw() const
{
}

void FrictionTestScene::OnSceneActivated()
{
}

void FrictionTestScene::OnSceneDeactivated()
{
}
