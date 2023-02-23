#include "stdafx.h"
#include "BouncingBallScene.h"
#include "SpherePosColorNorm.h"
#include "Logger.h"

void BouncingBallScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };
	const PxMaterial* pDefaultMaterial{ pPhysX->createMaterial(.5f, .5f, .8f) };

	//CAMERA
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ .0f, 10.f, -30.f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ .0f, -.265f, 1.f });

	//GROUND PLANE
	const auto pGroundActor{ pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{ .0f, .0f, 1.f } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//BALLS
	const float xStart{ -((m_pBalls.size() - 1) * m_Separation) * .5f };

	for (int i{ 0 }; i < m_pBalls.size(); ++i)
	{
		m_pBalls[i] = new SpherePosColorNorm(m_BallRadius, 20, 20, XMFLOAT4{ Colors::Green });
		AddGameObject(m_pBalls[i]);

		PxRigidDynamic* pBallActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
		PxSphereGeometry sphereGeometry{ m_BallRadius };

		// Create a material based on the amount of balls, the ball at index 0 should have a restitution of one and the last index a restitution of zero
		const PxMaterial* pMaterial{ pPhysX->createMaterial(.5f, .5f, static_cast<float>(m_pBalls.size() - i - 1) / static_cast<float>(m_pBalls.size() - 1)) };

		PxRigidActorExt::createExclusiveShape(*pBallActor, sphereGeometry, *pMaterial);

		m_pBalls[i]->AttachRigidActor(pBallActor);

		m_pBalls[i]->Translate(xStart + (static_cast<float>(i) * m_Separation), 10.f, .0f);
	}
}

void BouncingBallScene::Update()
{
	//RESET
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		//BALLS
		const float xStart{ -((m_pBalls.size() - 1) * m_Separation) * .5f };

		for (int i{ 0 }; i < m_pBalls.size(); ++i)
		{
			m_pBalls[i]->Translate(xStart + (static_cast<float>(i) * m_Separation), 10.f, .0f);
		}

		//CAMERA
		m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ .0f, 10.f, -30.f });
		m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ .0f, -.265f, 1.f });
	}
}

void BouncingBallScene::Draw() const
{
}

void BouncingBallScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"\t[INPUT > Reset='R']");
}

void BouncingBallScene::OnSceneDeactivated()
{
}
