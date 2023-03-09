//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "PongScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"

PongScene::PongScene() :
	GameScene(L"PongScene") {}

void PongScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.clearColor = XMFLOAT4{ Colors::Black };

	auto& pPhysX{ PxGetPhysics() };
	auto pBouncyMat{ pPhysX.createMaterial(.0f, .0f, 1.f) };

	CreateCamera();
	CreateWalls(pBouncyMat);
	CreateTriggers(pBouncyMat);
	CreatePaddles(pBouncyMat);
	CreateBall(pBouncyMat);
}

void PongScene::Update()
{
	if (m_ResetScene)
	{
		ResetScene();
	}
}

void PongScene::Draw()
{
	//Optional
}

void PongScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}

void PongScene::CreateCamera()
{
	m_pCamera = new FixedCamera();
	m_pCamera->GetTransform()->Translate(0, 25, 0);
	m_pCamera->GetTransform()->Rotate(90, 0, 0);
	AddChild(m_pCamera);

	SetActiveCamera(m_pCamera->GetComponent<CameraComponent>());
}

void PongScene::CreatePaddles(const PxMaterial* material)
{
	constexpr float paddleWidth{ 4.f };

	// Left Paddle
	m_pLeftPaddle = new CubePrefab(1, 1, paddleWidth, XMFLOAT4(Colors::White));
	m_pLeftPaddle->GetTransform()->Translate(-15, 0, 0);
	AddChild(m_pLeftPaddle);

	// Rigid body component
	auto pRigidBody{ m_pLeftPaddle->AddComponent(new RigidBodyComponent(false)) };
	pRigidBody->AddCollider(PxBoxGeometry{ .5f, .5f, paddleWidth * .5f }, *material);
	pRigidBody->SetKinematic(true);

	// Right Paddle
	m_pRightPaddle = new CubePrefab(1, 1, paddleWidth, XMFLOAT4(Colors::White));
	m_pRightPaddle->GetTransform()->Translate(15, 0, 0);
	AddChild(m_pRightPaddle);

	// Rigid body component
	pRigidBody = m_pRightPaddle->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxBoxGeometry{ .5f, .5f, paddleWidth * .5f }, *material);
	pRigidBody->SetKinematic(true);
}

void PongScene::CreateTriggers(const PxMaterial* material)
{
	constexpr float triggerWidth{ 23.f };

	// Left Trigger
	m_pLeftTrigger = new GameObject();
	m_pLeftTrigger->GetTransform()->Translate(-19, 0, 0);
	AddChild(m_pLeftTrigger);

	// Rigid body component
	auto pRigidBody{ m_pLeftTrigger->AddComponent(new RigidBodyComponent(false)) };
	pRigidBody->AddCollider(PxBoxGeometry{ .5f, .5f, triggerWidth * .5f }, *material, true);
	pRigidBody->SetKinematic(true);

	// Right Trigger
	m_pRightTrigger = new GameObject();
	m_pRightTrigger->GetTransform()->Translate(19, 0, 0);
	AddChild(m_pRightTrigger);

	// Rigid body component
	pRigidBody = m_pRightTrigger->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxBoxGeometry{ .5f, .5f, triggerWidth * .5f }, *material, true);
	pRigidBody->SetKinematic(true);

	auto triggerCallBack =
		std::bind(
			&PongScene::OnTrigger, this,
			std::placeholders::_1,
			std::placeholders::_2,
			std::placeholders::_3
		);
	m_pLeftTrigger->SetOnTriggerCallBack(triggerCallBack);
	m_pRightTrigger->SetOnTriggerCallBack(triggerCallBack);
}

void PongScene::CreateWalls(const PxMaterial* material)
{
	// Top Wall
	auto pLeftWall{ new CubePrefab(38, 1, 1, XMFLOAT4(Colors::Red)) };
	pLeftWall->GetTransform()->Translate(0, 0, 11);
	AddChild(pLeftWall);

	// Rigid body component
	auto pRigidBody{ pLeftWall->AddComponent(new RigidBodyComponent(false)) };
	pRigidBody->AddCollider(PxBoxGeometry{ 38.f * .5f, .5f, .5f }, *material);
	pRigidBody->SetKinematic(true);

	// Right Wall
	auto pRightWall{ new CubePrefab(38, 1, 1, XMFLOAT4(Colors::Red)) };
	pRightWall->GetTransform()->Translate(0, 0, -11);
	AddChild(pRightWall);

	// Rigid body component
	pRigidBody = pRightWall->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxBoxGeometry{ 38.f * .5f, .5f, .5f }, *material);
	pRigidBody->SetKinematic(true);
}

void PongScene::OnTrigger(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
{
	if (action != PxTriggerAction::ENTER) return;

	if (pOtherObject != m_pBall) return;

	m_ResetScene = pTriggerObject == m_pLeftTrigger || pTriggerObject == m_pRightTrigger;
}

void PongScene::ResetScene()
{
	// Translate the ball to the middle of the screen
	m_pBall->GetTransform()->Translate(0, .5f, 0);
	m_pBall->GetComponent<RigidBodyComponent>()->AddForce({ 5, 0, 5 }, PxForceMode::eIMPULSE);

	m_ResetScene = false;
}

void PongScene::CreateBall(const PxMaterial* material)
{
	m_pBall = new SpherePrefab(1, 16, XMFLOAT4(Colors::Red));
	m_pBall->GetTransform()->Translate(0, .5f, 0);
	AddChild(m_pBall);

	// Rigid body component
	auto pRigidBody{ m_pBall->AddComponent(new RigidBodyComponent(false)) };
	pRigidBody->AddCollider(PxSphereGeometry{ 1.f }, *material);

	auto& pPhysX{ PxGetPhysics() };

	auto pJoint{ PxD6JointCreate(pPhysX, nullptr, PxTransform{ PxIdentity }, pRigidBody->GetPxRigidActor(), PxTransform{ PxIdentity }) };
	pJoint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	pJoint->setMotion(PxD6Axis::eZ, PxD6Motion::eFREE);

	m_pBall->GetComponent<RigidBodyComponent>()->AddForce({ 5, 0, 5 }, PxForceMode::eIMPULSE);
}
