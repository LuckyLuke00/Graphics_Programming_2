#include "stdafx.h"
#include "Player.h"
#include "Input/ExamInput.h"

int Player::m_InputId{ -1 };
std::vector<XMVECTORF32> Player::m_ColorVariants{ Colors::White, Colors::Red, Colors::Blue, Colors::Yellow };

Player::Player(const std::wstring& model)
{
	++m_InputId;
	switch (m_InputId)
	{
	case 0:
		m_GamepadIndex = GamepadIndex::playerOne;
		break;
	case 1:
		m_GamepadIndex = GamepadIndex::playerTwo;
		break;
	case 2:
		m_GamepadIndex = GamepadIndex::playerThree;
		break;
	case 3:
		m_GamepadIndex = GamepadIndex::playerFour;
		break;
	}

	m_pModelComponent = AddComponent(new ModelComponent{ model });
	SetPlayerMaterials();
}

void Player::Initialize(const SceneContext&)
{
	// Add a GridMovementComponent
	m_pGridMovementComponent = AddComponent(new GridMovementComponent{});

	EnableInput();
}

void Player::OnSceneAttach(GameScene*)
{
	m_pModelAnimator = m_pModelComponent->GetAnimator();
	m_pModelAnimator->SetAnimationSpeed(1.f);
}

void Player::Update(const SceneContext&)
{
	HandleInput();

	HandleAnimations();
}

void Player::EnableInput() const
{
	InputManager* pInput{ GetScene()->GetSceneContext().pInput };

	using enum InputActions;

	InputAction north{ InputAction{ static_cast<int>(MoveNorth) + static_cast<int>(m_GamepadIndex) * 4, InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_UP, m_GamepadIndex } };
	pInput->AddInputAction(north);

	InputAction south{ InputAction{ static_cast<int>(MoveSouth) + static_cast<int>(m_GamepadIndex) * 4, InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, m_GamepadIndex } };
	pInput->AddInputAction(south);

	InputAction west{ InputAction{ static_cast<int>(MoveWest) + static_cast<int>(m_GamepadIndex) * 4, InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_LEFT, m_GamepadIndex } };
	pInput->AddInputAction(west);

	InputAction east{ InputAction{ static_cast<int>(MoveEast) + static_cast<int>(m_GamepadIndex) * 4, InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_RIGHT, m_GamepadIndex } };
	pInput->AddInputAction(east);
}

void Player::HandleInput() const
{
	if (HandleThumbstickInput()) return;

	using enum InputActions;

	const InputManager* pInput{ GetScene()->GetSceneContext().pInput };
	if (pInput->IsActionTriggered(static_cast<int>(MoveNorth) + static_cast<int>(m_GamepadIndex) * 4))
	{
		m_pGridMovementComponent->MoveNorth();
	}
	else if (pInput->IsActionTriggered(static_cast<int>(MoveSouth) + static_cast<int>(m_GamepadIndex) * 4))
	{
		m_pGridMovementComponent->MoveSouth();
	}
	else if (pInput->IsActionTriggered(static_cast<int>(MoveWest) + static_cast<int>(m_GamepadIndex) * 4))
	{
		m_pGridMovementComponent->MoveWest();
	}
	else if (pInput->IsActionTriggered(static_cast<int>(MoveEast) + static_cast<int>(m_GamepadIndex) * 4))
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

	if (const float angle{ atan2f(movement.y, movement.x) };
		angle > -XM_PIDIV4 && angle < XM_PIDIV4)
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

void Player::SetPlayerMaterials()
{
	// Get a random color from the color variants, set it as the body color and remove it from the vector
	const int colorIndex{ MathHelper::randI(0, static_cast<int>(m_ColorVariants.size()) - 1) };
	const XMVECTORF32 color{ m_ColorVariants[colorIndex] };
	m_ColorVariants.erase(m_ColorVariants.begin() + colorIndex);

	m_pBlack = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	m_pBlack->SetColor(Colors::Black);

	m_pEyebrow = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	m_pEyebrow->SetColor(Colors::DarkRed);

	m_pEyes = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	m_pEyes->SetColor(Colors::DarkBlue);

	m_pFace = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	m_pFace->SetColor(Colors::LightYellow);

	m_pGold = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	m_pGold->SetColor(Colors::Gold);

	m_pPink = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	m_pPink->SetColor(Colors::HotPink);

	m_pTShirt = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	m_pTShirt->SetColor(color);

	m_pBody = MaterialManager::Get()->CreateMaterial<ColorMaterial_Shadow_Skinned>();
	m_pBody->SetColor(color);

	m_pModelComponent->SetMaterial(m_pBlack, 0u); // Eyebrows
	m_pModelComponent->SetMaterial(m_pBlack, 2u); // Belt
	m_pModelComponent->SetMaterial(m_pBlack, 3u); // Eyes

	m_pModelComponent->SetMaterial(m_pFace, 12u); // Face

	m_pModelComponent->SetMaterial(m_pGold, 6u); // Belt Buckle

	m_pModelComponent->SetMaterial(m_pPink, 1u); // Hands
	m_pModelComponent->SetMaterial(m_pPink, 5u); // Antenna Sphere
	m_pModelComponent->SetMaterial(m_pPink, 8u); // Shoes

	m_pModelComponent->SetMaterial(m_pTShirt, 7u); // T-Shirt

	m_pModelComponent->SetMaterial(m_pBody, 10u); // Helmet
	m_pModelComponent->SetMaterial(m_pBody, 11u); // Legs
	m_pModelComponent->SetMaterial(m_pBody, 4u); // Arms
	m_pModelComponent->SetMaterial(m_pBody, 9u); // Antenna
}

void Player::HandleAnimations()
{
	if (!m_pModelAnimator->IsPlaying())
	{
		m_pModelAnimator->SetAnimation(static_cast<UINT>(m_AnimationState));
		m_pModelAnimator->Play();
		return;
	}

	if (m_pModelAnimator->GetCurrentClip().name != m_pModelAnimator->GetClip(static_cast<UINT>(m_AnimationState)).name)
	{
		m_pModelAnimator->SetAnimation(static_cast<UINT>(m_AnimationState));
		m_pModelAnimator->Play();
	}
}
