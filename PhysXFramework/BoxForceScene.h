#pragma once
#include "GameScene.h"

class BoxForceScene final : public GameScene
{
public:
	BoxForceScene() :GameScene(L"BoxForceScene") {}
	~BoxForceScene() override = default;
	BoxForceScene(const BoxForceScene& other) = delete;
	BoxForceScene(BoxForceScene&& other) noexcept = delete;
	BoxForceScene& operator=(const BoxForceScene& other) = delete;
	BoxForceScene& operator=(BoxForceScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	const float m_BoxSize{ 2.f };
	GameObject* m_pBox{ nullptr };
};
