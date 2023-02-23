#include "stdafx.h"
#include "BaseCamera.h"
#include "FreeCamera.h"

using namespace DirectX;

BaseCamera::BaseCamera()
	: m_World{}
	, m_View{}
	, m_Projection{}
	, m_ViewProjection{}
	, m_Up(XMFLOAT3(0, 1, 0))
	, m_Right(XMFLOAT3(1, 0, 0))
	, m_Forward(XMFLOAT3(0, 0, 1))
	, m_Position(XMFLOAT3(0, 10, -50))
	, m_Target(XMFLOAT3(0, 0, 0))
{
	XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_Projection, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, DirectX::XMMatrixIdentity());
}

//void BaseCamera::BuildView(const FXMVECTOR& eyePos, const FXMVECTOR& eyeFocus, const FXMVECTOR& up)
//{
//	XMStoreFloat3(&m_EyePos, eyePos);
//	XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(eyePos, eyeFocus, up));
//
//	BuildViewProjection();
//}

void BaseCamera::SetPositionAndLookAt(const XMFLOAT3& pos, const XMFLOAT3& lookAt)
{
	m_Position = pos;

	XMVECTOR vPos = XMLoadFloat3(&m_Position);
	XMVECTOR vLookAt = XMLoadFloat3(&lookAt);
	XMStoreFloat3(&m_Forward, XMVectorSubtract(vLookAt, vPos));
}

void BaseCamera::BuildView()
{
	XMVECTOR pos = XMLoadFloat3(&m_Position);
	XMVECTOR forward = XMLoadFloat3(&m_Forward);
	XMVector3Normalize(forward);

	auto target = pos + forward;

	XMStoreFloat3(&m_Target, target);

	XMStoreFloat4x4(&m_View, DirectX::XMMatrixLookAtLH(pos, target, XMLoadFloat3(&m_Up)));
	BuildViewProjection();
}

void BaseCamera::BuildProjection(float fov, float aspectRatio, float nearZ, float farZ)
{
	XMStoreFloat4x4(&m_Projection, DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ));
	BuildViewProjection();
}

void BaseCamera::BuildViewProjection()
{
	auto proj = XMLoadFloat4x4(&m_Projection);
	auto view = XMLoadFloat4x4(&m_View);

	XMStoreFloat4x4(&m_ViewProjection, view * proj);
}
