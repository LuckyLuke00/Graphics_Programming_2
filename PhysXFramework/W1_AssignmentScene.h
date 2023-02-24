#pragma once
#include "GameScene.h"

class W1_AssignmentScene final : public GameScene
{
public:
	W1_AssignmentScene() :GameScene(L"W1_AssignmentScene") {}
	~W1_AssignmentScene() override = default;
	W1_AssignmentScene(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene(W1_AssignmentScene&& other) noexcept = delete;
	W1_AssignmentScene& operator=(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene& operator=(W1_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	const float m_BallMass{ 50.f };
	const float m_BallRadius{ 1.f };
	const float m_BoxSize{ 1.f };
	const float m_FloorSize{ 50.f };

	const int m_Boxes{ 100 };
	const int m_VerticalBoxes{ 10 };
	const float m_Separation{ .25f };
	const float m_yOffset{ .25f };

	std::vector<GameObject*> m_pWall;

	// Game Objects
	GameObject* m_pFloor{ nullptr };
	GameObject* m_pBall{ nullptr };

	// Function that creates a wall of boxes takes in a position, the amount of boxes, the material the PxPhysx
	void CreateWall(const int boxes, const int verticalBoxes, const float separation, const PxMaterial* pMaterial, PxPhysics* pPhysX);
	void ResetWall(const int boxes, const int verticalBoxes, const float separation);
};
