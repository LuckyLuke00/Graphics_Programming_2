#include "stdafx.h"
#include "Player.h"
#include <Materials/DiffuseMaterial.h>

int Player::m_InputId{ -1 };

Player::Player(const std::wstring& model, const std::wstring& texture)
{
	++m_InputId;
	m_GamepadIndex = static_cast<GamepadIndex>(m_InputId);

	DiffuseMaterial* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	if (!texture.empty()) pMaterial->SetDiffuseTexture(texture);

	auto* pModel{ AddComponent(new ModelComponent{ model }) };
	pModel->SetMaterial(pMaterial);
}

void Player::Initialize(const SceneContext&)
{
	// Add a GridMovementComponent
	m_pGridMovementComponent = AddComponent(new GridMovementComponent{});
}

void Player::Update(const SceneContext&)
{
	HandleInput();
}

void Player::EnableInput() const
{
	using enum Player::InputActions;

	//// Store the pInput
	//InputManager* pInput{ GetScene()->GetSceneContext().pInput };

	//

	// Check if the thumbstick is moved
}

void Player::HandleInput() const
{
	auto movement{ InputManager::GetThumbstickPosition(true, m_GamepadIndex) };

	// If movement is nearly zero, do nothing
	if (abs(movement.x) < 0.1f && abs(movement.y) < 0.1f) return;

	// Calculate the angle between x and y
	const float angle{ atan2f(movement.y, movement.x) };

	// If the angle is between -45 and 45 degrees, move east
	if (angle > -XM_PIDIV4 && angle < XM_PIDIV4)
	{
		m_pGridMovementComponent->MoveEast();
	}
	else if (angle > XM_PIDIV4 && angle < XM_PIDIV4 * 3)
	{
		m_pGridMovementComponent->MoveNorth();
	}
	else if (angle < -XM_PIDIV4 && angle > -XM_PIDIV4 * 3)
	{
		m_pGridMovementComponent->MoveSouth();
	}
	else
	{
		m_pGridMovementComponent->MoveWest();
	}
}
