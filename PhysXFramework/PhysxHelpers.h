#pragma once
//DirectXMath Header Files:
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>
using namespace DirectX;

inline PxVec3 ToPxVec3(const DirectX::XMFLOAT3& vec)
{
	return PxVec3(vec.x, vec.y, vec.z);
}

inline PxVec2 ToPxVec2(const DirectX::XMFLOAT2& vec)
{
	return PxVec2(vec.x, vec.y);
}

inline PxVec4 ToPxVec4(const DirectX::XMFLOAT4& vec)
{
	return PxVec4(vec.x, vec.y, vec.z, vec.w);
}

inline PxQuat ToPxQuat(const DirectX::XMFLOAT4& vec)
{
	return PxQuat(vec.x, vec.y, vec.z, vec.w);
}
inline DirectX::XMFLOAT3 ToXMFLOAT3(const PxVec3& vec)
{
	return DirectX::XMFLOAT3(vec.x, vec.y, vec.z);
}
