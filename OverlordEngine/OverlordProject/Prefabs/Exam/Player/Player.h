#pragma once
#include "Prefabs/Exam/Level/GridObject.h"
#include <Components/GridMovementComponent.h>
#include <Materials/Shadow/ColorMaterial_Shadow_Skinned.h>

class Player : public GridObject
{
public:
	explicit Player(const std::wstring& model);
	~Player() override = default;

	Player(const Player& other) = delete;
	Player(Player&& other) noexcept = delete;
	Player& operator=(const Player& other) = delete;
	Player& operator=(Player&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void OnSceneAttach(GameScene* /*pScene*/) override;
	void Update(const SceneContext& sceneContext) override;

private:
	enum class AnimationState : UINT
	{
		Floating,
		Run,
		Drop,
		Death,
		Idle,
		Lose,
		Dance
	};

	static int m_InputId;
	static std::vector<XMVECTORF32> m_ColorVariants;

	ColorMaterial_Shadow_Skinned* m_pBlack{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pEyebrow{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pEyes{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pFace{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pGold{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pPink{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pTShirt{ nullptr };
	ColorMaterial_Shadow_Skinned* m_pBody{ nullptr };

	AnimationState m_AnimationState{ AnimationState::Idle };
	GamepadIndex m_GamepadIndex{ GamepadIndex::playerOne };
	GridMovementComponent* m_pGridMovementComponent{ nullptr };
	ModelAnimator* m_pModelAnimator{ nullptr };
	ModelComponent* m_pModelComponent{ nullptr };

	bool HandleThumbstickInput() const;
	void EnableInput() const;
	void HandleInput() const;
	void SetPlayerMaterials();
	void HandleAnimations();
};
