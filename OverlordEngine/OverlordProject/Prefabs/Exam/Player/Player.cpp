#include "stdafx.h"
#include "Player.h"
#include <Materials/DiffuseMaterial.h>
#include "Input/ExamInput.h"

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

	EnableInput();
}

void Player::Update(const SceneContext&)
{
	HandleInput();
}

void Player::EnableInput() const
{
	InputManager* pInput{ GetScene()->GetSceneContext().pInput };

	using enum InputActions;

	InputAction north{ InputAction{ static_cast<int>(MoveNorth), InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_UP, m_GamepadIndex } };
	pInput->AddInputAction(north);

	InputAction south{ InputAction{ static_cast<int>(MoveSouth), InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, m_GamepadIndex } };
	pInput->AddInputAction(south);

	InputAction west{ InputAction{ static_cast<int>(MoveWest), InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_LEFT, m_GamepadIndex } };
	pInput->AddInputAction(west);

	InputAction east{ InputAction{ static_cast<int>(MoveEast), InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_RIGHT, m_GamepadIndex } };
	pInput->AddInputAction(east);
}

void Player::HandleInput() const
{
	if (HandleThumbstickInput()) return;

	using enum InputActions;

	const InputManager* pInput{ GetScene()->GetSceneContext().pInput };
	if (pInput->IsActionTriggered(static_cast<int>(MoveNorth)))
	{
		m_pGridMovementComponent->MoveNorth();
	}
	else if (pInput->IsActionTriggered(static_cast<int>(MoveSouth)))
	{
		m_pGridMovementComponent->MoveSouth();
	}
	else if (pInput->IsActionTriggered(static_cast<int>(MoveWest)))
	{
		m_pGridMovementComponent->MoveWest();
	}
	else if (pInput->IsActionTriggered(static_cast<int>(MoveEast)))
	{
		m_pGridMovementComponent->MoveEast();
	}
}

bool Player::HandleThumbstickInput() const
{
	auto movement{ InputManager::GetThumbstickPosition(true, m_GamepadIndex) };

	// If movement is nearly zero, do nothing
	if (abs(movement.x) < 0.1f && abs(movement.y) < 0.1f) return false;

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

	return true;
}
