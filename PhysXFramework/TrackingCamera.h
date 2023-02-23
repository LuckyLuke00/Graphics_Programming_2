#pragma once
#include "BaseCamera.h"

class TrackingCamera final : public BaseCamera
{
public:
	TrackingCamera();
	~TrackingCamera() override = default;
	TrackingCamera(const TrackingCamera& t) = delete;
	TrackingCamera& operator=(const TrackingCamera& t) = delete;
	void BuildView();

	virtual void Update(const SceneContext& sceneContext)override;
	void SetPosition(DirectX::XMFLOAT3 cameraPosition);
	void SetTarget(DirectX::XMFLOAT3 targetPosition);

private:
	POINT m_PrevCursorPosition{};
	float m_TotalYaw{}, m_TotalPitch{};
	float m_Speed{ 30.f };
	bool m_MouseDownPreviousFrame{};
	DirectX::XMFLOAT3 m_Direction{};
};
