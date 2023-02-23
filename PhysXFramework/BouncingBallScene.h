#pragma once
#include "GameScene.h"
#include <array>

class BouncingBallScene final : public GameScene
{
public:
	BouncingBallScene() :GameScene(L"BouncingBallScene") {}
	~BouncingBallScene() override = default;
	BouncingBallScene(const BouncingBallScene& other) = delete;
	BouncingBallScene(BouncingBallScene&& other) noexcept = delete;
	BouncingBallScene& operator=(const BouncingBallScene& other) = delete;
	BouncingBallScene& operator=(BouncingBallScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	const float m_BallRadius{ 1.f };
	const float m_Separation{ 5.f };
	std::array<GameObject*, 3> m_pBalls{ nullptr };
};
