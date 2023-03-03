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

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

private:
	// Boxes
	const float m_BoxMass{ .15f };
	const float m_BoxSize{ 1.5f };
	GameObject* m_pBlueBox{ nullptr };
	GameObject* m_pRedBox{ nullptr };

	// Spheres
	const float m_SmallSphereRadius{ .75f };
	const float m_PlayerSphereRadius{ 1.f };
	GameObject* m_pBlueSphere{ nullptr };
	GameObject* m_pPlayerSphere{ nullptr };
	GameObject* m_pRedSphere{ nullptr };

	// Hatches
	const float m_HatchHeight{ .25f };
	const float m_HatchWidth{ 2.f };
	const float m_HatchDepth{ 5.5f };
	GameObject* m_pBlueHatch{ nullptr };
	GameObject* m_pRedHatch{ nullptr };

	// Trigger boxes
	const float m_TriggerWidth{ 1.f };
	GameObject* m_pBlueTriggerBox{ nullptr };
	GameObject* m_pRedTriggerBox{ nullptr };

	PxRevoluteJoint* m_pBlueHatchJoint{ nullptr };
	PxRevoluteJoint* m_pRedHatchJoint{ nullptr };

	// Triggers
	PxRigidStatic* m_pBlueTrigger{ nullptr };
	PxRigidStatic* m_pRedTrigger{ nullptr };

	// Level mesh
	GameObject* m_pLevelTriangle{ nullptr };

	bool m_IsTriggerBlue{ false };
	bool m_IsTriggerRed{ false };

	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Sound* m_pSound2D{ nullptr };

	void ResetScene();
};
