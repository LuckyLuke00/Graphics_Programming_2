#pragma once
#include "GameScene.h"

class W2_AssignmentScene final : public GameScene
{
public:
	W2_AssignmentScene() :GameScene(L"W2_AssignmentScene") {}
	~W2_AssignmentScene() override = default;
	W2_AssignmentScene(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene(W2_AssignmentScene&& other) noexcept = delete;
	W2_AssignmentScene& operator=(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene& operator=(W2_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	GameObject* m_pBlueSphere{ nullptr };
	GameObject* m_pLevelTriangle{ nullptr };
	GameObject* m_pPlayerSphere{ nullptr };
	GameObject* m_pRedSphere{ nullptr };

	const float m_PlayerSphereRadius{ 1.f };
	const float m_SmallSphereRadius{ .75f };
};
