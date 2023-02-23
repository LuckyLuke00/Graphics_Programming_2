#include "stdafx.h"
#include "FreeCamera.h"
#include <Windows.h>
#include <iostream>
#include "SceneContext.h"
#include "Core.h"
#include "Logger.h"

FreeCamera::FreeCamera()
	: m_TotalYaw{ 0 }
	, m_TotalPitch{ 0 }
	, m_Direction{ DirectX::XMFLOAT3{0,0,1} }
	, m_Speed{ 30.0f }
	, m_MouseDownPreviousFrame{ false }
	, m_UpKey{ 'W' }
	, m_LeftKey{ 'A' }
{
	auto& gameSettings = Core::GetGameSettings();
	BuildProjection(XM_PIDIV4, gameSettings.GetAspectRatio(), 0.01f, 1000.0f);
}

void FreeCamera::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.GetGameTime()->GetElapsed();
	auto input = sceneContext.GetInput();

	XMVECTOR forward = XMLoadFloat3(&m_Forward);
	if (input->IsMouseButton(InputTriggerState::down, VK_LBUTTON))
	{
		auto cursorMovement = input->GetMouseMovement();
		float xDiff = cursorMovement.x / 200.0f;
		float yDiff = cursorMovement.y / 200.0f;

		//Logger::GetInstance()->LogDebug(std::to_wstring(xDiff) + std::to_wstring(yDiff));

		m_TotalYaw += xDiff * XMConvertToRadians(45);
		m_TotalPitch += yDiff * XMConvertToRadians(45);

		DirectX::XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(m_TotalPitch, m_TotalYaw, 0);

		XMVECTOR unitZ = { 0,0,1.0f,0 };
		forward = XMVector3TransformCoord(unitZ, rotMat);
	}

	forward = XMVector3Normalize(forward);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);

	XMVECTOR right = XMVector3Cross(up, forward);
	right = XMVector3Normalize(right);

	up = XMVector3Cross(forward, right);
	up = XMVector3Normalize(up);

	XMVECTOR position = XMLoadFloat3(&m_Position);

	if (input->IsKeyboardKey(InputTriggerState::down, m_UpKey))
		position += forward * m_Speed * deltaTime;
	else if (input->IsKeyboardKey(InputTriggerState::down, 'S'))
		position -= forward * m_Speed * deltaTime;

	if (input->IsKeyboardKey(InputTriggerState::down, m_LeftKey))
		position -= right * m_Speed * deltaTime;
	else if (input->IsKeyboardKey(InputTriggerState::down, 'D'))
		position += right * m_Speed * deltaTime;

	XMStoreFloat3(&m_Position, position);
	XMStoreFloat3(&m_Forward, forward);
	XMStoreFloat3(&m_Up, up);
	XMStoreFloat3(&m_Right, right);
	BuildView();
}
