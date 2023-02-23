#pragma once
#include "GameScene.h"

class BoxTorqueScene final : public GameScene
{
public:
	BoxTorqueScene() :GameScene(L"BoxTorqueScene") {}
	~BoxTorqueScene() override = default;
	BoxTorqueScene(const BoxTorqueScene& other) = delete;
	BoxTorqueScene(BoxTorqueScene&& other) noexcept = delete;
	BoxTorqueScene& operator=(const BoxTorqueScene& other) = delete;
	BoxTorqueScene& operator=(BoxTorqueScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	const float m_BoxSize{ 3.f };
	GameObject* m_pBox{ nullptr };
};
