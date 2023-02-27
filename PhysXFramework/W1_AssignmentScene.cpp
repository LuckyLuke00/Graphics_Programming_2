#include "stdafx.h"

#include "W1_AssignmentScene.h"
#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"
#include "Logger.h"

#include <vector>

enum class InputIds : int
{
	Up,
	Down,
	Left,
	Right,
	Jump
};

void W1_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	//CAMERA
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ -20.f, 10.f, -30.f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ .5f, -.25f, 1.f });

	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };

	// Materials
	const PxMaterial* pFloorMaterial{ pPhysX->createMaterial(.6f, .6f, .2f) };
	const PxMaterial* pWallMaterial{ pPhysX->createMaterial(.7f, .7f, .4f) };
	const PxMaterial* pSphereMaterial{ pPhysX->createMaterial(.9f, .9f, .8f) };

	//FLOOR
	m_pFloor = new CubePosColorNorm(m_FloorSize, .0f, m_FloorSize, XMFLOAT4{ Colors::LightGray });
	PxRigidStatic* pFloorActor{ pPhysX->createRigidStatic(PxTransform{ PxIdentity }) };
	PxBoxGeometry floorGeometry{ PxBoxGeometry{ m_FloorSize * .5f, .0f, m_FloorSize * .5f } };
	PxRigidActorExt::createExclusiveShape(*pFloorActor, floorGeometry, *pFloorMaterial);
	m_pFloor->AttachRigidActor(pFloorActor);
	AddGameObject(m_pFloor);

	//WALLS
	CreateWall(m_Boxes, m_VerticalBoxes, m_Separation, pWallMaterial, pPhysX);

	//BALL
	m_pBall = new SpherePosColorNorm(m_BallRadius, 20, 20, XMFLOAT4{ Colors::Yellow });
	PxRigidDynamic* pBallActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxSphereGeometry sphereGeometry{ m_BallRadius };
	PxRigidActorExt::createExclusiveShape(*pBallActor, sphereGeometry, *pSphereMaterial);

	// Set the mass of the ball by using PxRigidyBodyExt
	PxRigidBodyExt::setMassAndUpdateInertia(*pBallActor, m_BallMass);

	m_pBall->AttachRigidActor(pBallActor);
	AddGameObject(m_pBall);

	// Set the ball in the middle of the floor
	m_pBall->Translate(.0f, m_BallRadius, .0f);

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

	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
			static_cast<int>(InputIds::Jump),
			InputTriggerState::pressed,
			VK_SPACE,
			-1,
			XINPUT_GAMEPAD_A
		});
}

void W1_AssignmentScene::Update()
{
	const float speed{ 10.f * m_BallMass };

	// Calculate torque based on the camera forward and right vectors
	const XMFLOAT3 forward{ m_SceneContext.GetCamera()->GetForward() };
	const XMFLOAT3 right{ m_SceneContext.GetCamera()->GetRight() };

	// Convert to normalized PhysX vectors
	const PxVec3 forwardVec{ PxVec3{ forward.x, forward.y, forward.z }.getNormalized() };
	const PxVec3 rightVec{ PxVec3{ right.x, right.y, right.z }.getNormalized() };

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Up)))
	{
		// Use ForwardVec
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addTorque(PxVec3{ rightVec * speed });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Down)))
	{
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addTorque(PxVec3{ rightVec * -speed });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Left)))
	{
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addTorque(PxVec3{ forwardVec * speed });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Right)))
	{
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addTorque(PxVec3{ forwardVec * -speed });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Jump)))
	{
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->addForce(PxVec3{ .0f, speed * m_BallMass, .0f });
	}

	// Reset
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		ResetWall(m_Boxes, m_VerticalBoxes, m_Separation);

		// Reset the ball
		m_pBall->Translate(.0f, m_BallRadius, .0f);
		m_pBall->RotateDegrees(.0f, .0f, .0f);
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->setAngularVelocity(PxVec3{ .0f, .0f, .0f });
		static_cast<PxRigidBody*>(m_pBall->GetRigidActor())->setLinearVelocity(PxVec3{ .0f, .0f, .0f });
	}
}

void W1_AssignmentScene::Draw() const
{
}

void W1_AssignmentScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"\t[INPUT > Reset='R' | Apply_Torque='Arrow Keys' | Apply_Upwards_Force='SPACE']");
}

void W1_AssignmentScene::OnSceneDeactivated()
{
}

void W1_AssignmentScene::CreateWall(const int boxes, const int verticalBoxes, const float separation, const PxMaterial* pMaterial, PxPhysics* pPhysX)
{
	m_pWall.reserve(boxes);

	const float xStart{ -((boxes / verticalBoxes - 1) * (separation + m_BoxSize)) * .5f };

	for (int i{ 0 }; i < boxes; ++i)
	{
		m_pWall.emplace_back(new CubePosColorNorm(m_BoxSize, m_BoxSize, m_BoxSize));
		PxRigidDynamic* pBoxActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
		PxBoxGeometry boxGeometry{ PxBoxGeometry{ m_BoxSize * .5f, m_BoxSize * .5f, m_BoxSize * .5f } };
		PxRigidActorExt::createExclusiveShape(*pBoxActor, boxGeometry, *pMaterial);
		m_pWall.back()->AttachRigidActor(pBoxActor);
		AddGameObject(m_pWall.back());

		// Calculate x position
		const float x{ xStart + (i % (boxes / verticalBoxes)) * (separation + m_BoxSize) };

		// Calculate y position
		const float y{ m_BoxSize * .5f + (i / (boxes / verticalBoxes)) * m_BoxSize + m_yOffset };

		// Calculate z position (at the back of the floor)
		const float back{ m_FloorSize * .5f - m_BoxSize * .5f };
		// A quarter of the way back
		const float z{ back - back * .5f };

		m_pWall.back()->Translate(x, y, z);

		// Give them a small random rotation around the y-axis
		m_pWall.back()->RotateDegrees(.0f, static_cast<float>(rand() % 25), .0f);
	}
}

void W1_AssignmentScene::ResetWall(const int boxes, const int verticalBoxes, const float separation)
{
	const float xStart{ -((boxes / verticalBoxes - 1) * (separation + m_BoxSize)) * .5f };

	for (int i{ 0 }; i < boxes; ++i)
	{
		// Calculate x position
		const float x{ xStart + (i % (boxes / verticalBoxes)) * (separation + m_BoxSize) };

		// Calculate y position
		const float y{ m_BoxSize * .5f + (i / (boxes / verticalBoxes)) * m_BoxSize + m_yOffset };

		// Calculate z position (at the back of the floor)
		const float back{ m_FloorSize * .5f - m_BoxSize * .5f };
		// A quarter of the way back
		const float z{ back - back * .5f };

		m_pWall[i]->Translate(x, y, z);

		// Give them a small random rotation around the y-axis
		m_pWall[i]->RotateDegrees(.0f, static_cast<float>(rand() % 25), .0f);
	}
}
