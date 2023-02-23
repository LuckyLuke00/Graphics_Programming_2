#include "stdafx.h"
#include "TestScene.h"

#include "CubePosColorNorm.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"
#include "Logger.h"

enum class InputIds : int
{
	Up,
	Down,
	Left,
	Right
};

void TestScene::Initialize()
{
	//CUBE
	m_pBox = new CubePosColorNorm(5.f, 2.f, 1.f);
	AddGameObject(m_pBox);

	m_pBox->Translate(.0f, .0f, .0f);
	m_pBox->RotateDegrees(.0f, 45.f, .0f);

	//CAMERA
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ .0f, 10.f, -25.f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ .0f, -.3f, 1.f });

	//TORUS
	m_pTorus = new TorusPosColorNorm(8.f, 50, 1.f, 10, XMFLOAT4{ Colors::Green });
	AddGameObject(m_pTorus);

	m_pCylinder = new CylinderPosColorNorm(1.5f, 10, 4.f, XMFLOAT4{ Colors::Blue });
	m_pTorus->AddChild(m_pCylinder);

	m_pCylinder->Translate(.0f, 8.f, .0f);

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

void TestScene::Update()
{
	const float totalTime{ m_SceneContext.GetGameTime()->GetTotal() };
	m_pBox->RotateDegrees(.0f, 90.f * totalTime, .0f);

	XMFLOAT3 translation{};
	const float movementSpeed{ 10.f * m_SceneContext.GetGameTime()->GetElapsed() };

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Up)))
	{
		translation.y += movementSpeed;
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Down)))
	{
		translation.y -= movementSpeed;
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Left)))
	{
		translation.x -= movementSpeed;
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(static_cast<int>(InputIds::Right)))
	{
		translation.x += movementSpeed;
	}

	XMStoreFloat3(&translation, XMVectorAdd(XMLoadFloat3(&translation), XMLoadFloat3(&m_pBox->GetPosition())));
	m_pBox->Translate(translation.x, translation.y, translation.z);
}

void TestScene::Draw() const
{
}

void TestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"\t[INPUT > Translate = 'Arrow Keys']", GetName().c_str());
}

void TestScene::OnSceneDeactivated()
{
}
