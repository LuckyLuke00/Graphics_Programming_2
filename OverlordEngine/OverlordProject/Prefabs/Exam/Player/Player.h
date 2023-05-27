#pragma once
#include "Prefabs/Exam/Level/GridObject.h"
#include <Components/GridMovementComponent.h>

class Player : public GridObject
{
public:
	explicit Player(const std::wstring& model, const std::wstring& texture);
	~Player() override = default;

	Player(const Player& other) = delete;
	Player(Player&& other) noexcept = delete;
	Player& operator=(const Player& other) = delete;
	Player& operator=(Player&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:

	static int m_InputId;
	GamepadIndex m_GamepadIndex{ GamepadIndex::playerOne };
	GridMovementComponent* m_pGridMovementComponent{ nullptr };

	void EnableInput() const;
	void HandleInput() const;
	bool HandleThumbstickInput() const;
};
