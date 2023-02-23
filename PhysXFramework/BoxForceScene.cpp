#include "stdafx.h"
#include "BoxForceScene.h"
#include "CubePosColorNorm.h"
#include "DebugRenderer.h"
#include "Logger.h"

enum class InputIds : int
{
	Up,
	Down,
	Left,
	Right
};

void BoxForceScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };
	const PxMaterial* pDefaultMaterial{ pPhysX->createMaterial(.2f, .2f, .2f) };

	//GROUND PLANE
	const auto pGroundActor{ pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{ .0f, .0f, 1.f } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//CAMERA
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ .0f, 40.f, -20.f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ .0f, -2.f, 1.f });

	//CUBE
	const XMFLOAT3 actorDimensions{ 2.f, 2.f, 2.f };
	m_pBox = new CubePosColorNorm(actorDimensions.x, actorDimensions.y, actorDimensions.z);
	PxRigidDynamic* pBoxActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxBoxGeometry boxGeometry{ PxBoxGeometry{ actorDimensions.x * .5f, actorDimensions.y * .5f, actorDimensions.z * .5f } };
	PxRigidActorExt::createExclusiveShape(*pBoxActor, boxGeometry, *pDefaultMaterial);
	m_pBox->AttachRigidActor(pBoxActor);
	AddGameObject(m_pBox);

	m_pBox->Translate(0.f, actorDimensions.y * .5f, 0.f);

	//INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Up),
			InputTriggerState::down,
			VK_UP,
			-1,
			XINPUT_GAMEPAD_DPAD_UP
		});

	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Down),
			InputTriggerState::down,
			VK_DOWN,
			-1,
			XINPUT_GAMEPAD_DPAD_DOWN
		});

	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Left),
			InputTriggerState::down,
			VK_LEFT,
			-1,
			XINPUT_GAMEPAD_DPAD_LEFT
		});

	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Right),
			InputTriggerState::down,
			VK_RIGHT,
			-1,
			XINPUT_GAMEPAD_DPAD_RIGHT
		});
}

void BoxForceScene::Update()
{
	constexpr float speed{ 10.f };
	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Up)))
	{
		static_cast<PxRigidBody*>(m_pBox->GetRigidActor())->addForce(PxVec3{ 0.f, 0.f, speed });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Down)))
	{
		static_cast<PxRigidBody*>(m_pBox->GetRigidActor())->addForce(PxVec3{ 0.f, 0.f, -speed });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Left)))
	{
		static_cast<PxRigidBody*>(m_pBox->GetRigidActor())->addForce(PxVec3{ -speed, 0.f, 0.f });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Right)))
	{
		static_cast<PxRigidBody*>(m_pBox->GetRigidActor())->addForce(PxVec3{ speed, 0.f, 0.f });
	}
}

void BoxForceScene::Draw() const
{
}

void BoxForceScene::OnSceneActivated()
{
}

void BoxForceScene::OnSceneDeactivated()
{
}
