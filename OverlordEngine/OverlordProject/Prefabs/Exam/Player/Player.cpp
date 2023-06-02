#include "stdafx.h"
#include "Player.h"
#include "Components/PlaceBombComponent.h"
#include "Components/GridMovementComponent.h"
#include "Materials/Shadow/ColorMaterial_Shadow_Skinned.h"
#include "Prefabs/Exam/Level/GridMap.h"

int Player::m_InputId{ -1 };
std::vector<XMVECTORF32> Player::m_ColorVariants{ Colors::White, Colors::Red, Colors::Blue, Colors::Yellow };

Player::Player(const std::wstring& model)
{
	++m_InputId;
	m_GamepadIndex = m_InputId;

	m_pModelComponent = AddComponent(new ModelComponent{ model });
	SetPlayerMaterials();

	m_Lives = m_MaxLives;
}

void Player::Initialize(const SceneContext&)
{
	m_pPlaceBombComponent = AddComponent(new PlaceBombComponent{});
	m_pGridMovementComponent = AddComponent(new GridMovementComponent{ .5f });
	m_pPlaceBombComponent->SetGridMap(GetGridMap());
}

void Player::OnSceneAttach(GameScene*)
{
	m_pModelAnimator = m_pModelComponent->GetAnimator();
	m_pModelAnimator->SetAnimationSpeed(1.f);

	EnableInput();
}

void Player::Update(const SceneContext&)
{
	HandleInput();
	HandleAnimations();
	HandleDeath();
}

void Player::EnableInput() const
{
	InputManager* pInput{ GetScene()->GetSceneContext().pInput };

	using namespace ExamInput;
	using enum InputActions;

	InputAction north{ InputAction{ GetActionID(MoveNorth), InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_UP, static_cast<GamepadIndex>(m_GamepadIndex)} };
	pInput->AddInputAction(north);

	InputAction south{ InputAction{ GetActionID(MoveSouth), InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, static_cast<GamepadIndex>(m_GamepadIndex) } };
	pInput->AddInputAction(south);

	InputAction west{ InputAction{ GetActionID(MoveWest), InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_LEFT, static_cast<GamepadIndex>(m_GamepadIndex) } };
	pInput->AddInputAction(west);

	InputAction east{ InputAction{ GetActionID(MoveEast), InputState::down, -1, -1, XINPUT_GAMEPAD_DPAD_RIGHT, static_cast<GamepadIndex>(m_GamepadIndex) } };
	pInput->AddInputAction(east);

	InputAction placeBomb{ InputAction{GetActionID(PlaceBomb), InputState::pressed, -1, -1, XINPUT_GAMEPAD_A, static_cast<GamepadIndex>(m_GamepadIndex) } };
	pInput->AddInputAction(placeBomb);
}

void Player::HandleInput() const
{
	if (m_IsDead || HandleThumbstickInput()) return;

	using namespace ExamInput;
	using enum InputActions;

	const InputManager* pInput{ GetScene()->GetSceneContext().pInput };
	if (pInput->IsActionTriggered(GetActionID(MoveNorth)))
	{
		m_pGridMovementComponent->MoveNorth();
	}
	else if (pInput->IsActionTriggered(GetActionID(MoveSouth)))
	{
		m_pGridMovementComponent->MoveSouth();
	}
	else if (pInput->IsActionTriggered(GetActionID(MoveWest)))
	{
		m_pGridMovementComponent->MoveWest();
	}
	else if (pInput->IsActionTriggered(GetActionID(MoveEast)))
	{
		m_pGridMovementComponent->MoveEast();
	}

	if (pInput->IsActionTriggered(GetActionID(PlaceBomb)))
	{
		m_pPlaceBombComponent->PlaceBomb();
	}
}

bool Player::HandleThumbstickInput() const
{
	auto movement{ InputManager::GetThumbstickPosition(true, static_cast<GamepadIndex>(m_GamepadIndex)) };

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

int Player::GetActionID(ExamInput::InputActions action) const
{
	return static_cast<int>(action) + m_GamepadIndex * ExamInput::PlayerActionCount;
}

void Player::HandleAnimations()
{
	if (m_IsDead)
	{
		m_AnimationState = AnimationState::Death;
	}
	else if (m_pGridMovementComponent->IsMoving())
	{
		m_AnimationState = AnimationState::Run;
	}
	else
	{
		m_AnimationState = AnimationState::Idle;
	}

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

void Player::HandleDeath()
{
	if (!m_IsDead) return;

	m_RespawnTimer += GetScene()->GetSceneContext().pGameTime->GetElapsed();
	if (m_RespawnTimer < m_RespawnTime) return;

	m_IsDead = false;
	m_RespawnTimer = 0.f;

	m_pGridMovementComponent->DecreaseMoveSpeed();

	if (m_Lives < 1)
	{
		GetGridMap()->AddDeadPlayer(this);
		GetGridMap()->RemovePlayer(this);
		MarkForDelete();
		return;
	}

	SetPosition(m_SpawnPoint.x, m_SpawnPoint.y);
}

void Player::Reset()
{
	m_InputId = -1;
	m_ColorVariants = { Colors::White, Colors::Red, Colors::Blue, Colors::Yellow };
}

void Player::Kill()
{
	if (m_IsDead) return;

	--m_Lives;
	m_IsDead = true;
}
