#include "stdafx.h"
#include "TrackingCamera.h"
#include "D3D11.h"
#include "SceneContext.h"
#include "Core.h"
TrackingCamera::TrackingCamera()
{
	auto& gameSettings = Core::GetGameSettings();
	BuildProjection(XM_PIDIV4, gameSettings.GetAspectRatio(), 0.01f, 1000.0f);
}

void TrackingCamera::Update(const SceneContext& sceneContext)
{
	float deltaTime = sceneContext.GetGameTime()->GetElapsed();

	XMVECTOR pos = XMLoadFloat3(&m_Position);
	XMVECTOR target = XMLoadFloat3(&m_Target);
	XMVECTOR vecForward = XMVector3Normalize(target - pos);
	XMStoreFloat3(&m_Forward, vecForward);

	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMVECTOR right = XMVector3Cross(vecForward, up);
	right = XMVector3Normalize(right);

	up = XMVector3Cross(right, vecForward);
	up = XMVector3Normalize(up);

	XMStoreFloat3(&m_Up, up);
	XMStoreFloat3(&m_Right, right);

	BuildView();
}

void TrackingCamera::SetPosition(DirectX::XMFLOAT3 cameraPosition)
{
	m_Position = cameraPosition;
}

void TrackingCamera::SetTarget(DirectX::XMFLOAT3 targetPosition)
{
	m_Target = targetPosition;
}

void TrackingCamera::BuildView()
{
	auto pos = XMLoadFloat3(&m_Position);
	auto target = XMLoadFloat3(&m_Target);

	XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(pos, target, XMLoadFloat3(&m_Up)));
	BuildViewProjection();
}
