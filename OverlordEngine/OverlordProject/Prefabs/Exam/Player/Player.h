#pragma once
#include "Components/GridMovementComponent.h"
#include "Components/PlaceBombComponent.h"
#include "Input/ExamInput.h"
#include "Materials/Shadow/ColorMaterial_Shadow_Skinned.h"
#include "Prefabs/Exam/Level/GridObject.h"

class GridMovementComponent;

class Player : public GridObject
{
public:
	explicit Player(const std::wstring& model);
	~Player() override = default;

	Player(const Player& other) = delete;
	Player(Player&& other) noexcept = delete;
	Player& operator=(const Player& other) = delete;
	Player& operator=(Player&& other) noexcept = delete;

	void SetSpawnPoint(const XMINT2& spawnPoint) { m_SpawnPoint = spawnPoint; }
	void SetSpawnPoint(int row, int col) { m_SpawnPoint.x = row; m_SpawnPoint.y = col; }
	void SetLives(int lives) { m_Lives = lives; }
	void Kill();

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void OnSceneAttach(GameScene* pScene) override;
	void Update(const SceneContext& sceneContext) override;

private:
	enum class AnimationState : UINT
	{
		Dance,
		Death,
		Drop,
		Floating,
		Idle,
		Lose,
		Run,
	};

	static int m_InputId;
	static std::vector<XMVECTORF32> m_ColorVariants;

	bool m_IsDead{ false };

	int m_GamepadIndex{ 0 };
	int m_Lives{ 3 };
	float m_RespawnTimer{ 0.f };
	float m_RespawnTime{ 2.f };

	ColorMaterial_Shadow_Skinned* m_pBlack{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pEyebrow{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pEyes{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pFace{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pGold{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pPink{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pTShirt{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pBody{ nullptr };

	AnimationState m_AnimationState{ AnimationState::Idle };
	GridMovementComponent* m_pGridMovementComponent{ nullptr };
	ModelAnimator* m_pModelAnimator{ nullptr };
	ModelComponent* m_pModelComponent{ nullptr };
	PlaceBombComponent* m_pPlaceBombComponent{ nullptr };

	XMINT2 m_SpawnPoint{ 1, 1 };

	bool HandleThumbstickInput() const;
	void EnableInput() const;
	void HandleAnimations();
	void HandleDeath();
	void HandleInput() const;
	void SetPlayerMaterials();

	int GetActionID(ExamInput::InputActions action) const;
};
