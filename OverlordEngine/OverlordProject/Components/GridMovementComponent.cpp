#include "stdafx.h"
#include "GridMovementComponent.h"
#include "Utils/MathHelper.h"

GridMovementComponent::GridMovementComponent(float moveSpeed) :
	m_MoveSpeed{ moveSpeed }
{}

void GridMovementComponent::MoveNorth()
{
	if (IsMoving()) return;
	if (m_pGridMap->IsOccupied(m_pGridMap->GetGridIndex({ m_TargetPosition.x, m_TargetPosition.y, m_TargetPosition.z + 1 }))) return;
	m_TargetPosition.z += 1;
	m_MoveTimer = m_MoveSpeed;
}

void GridMovementComponent::MoveSouth()
{
	if (IsMoving()) return;
	if (m_pGridMap->IsOccupied(m_pGridMap->GetGridIndex({ m_TargetPosition.x, m_TargetPosition.y, m_TargetPosition.z - 1 }))) return;
	m_TargetPosition.z -= 1;
	m_MoveTimer = m_MoveSpeed;
}

void GridMovementComponent::MoveEast()
{
	if (IsMoving()) return;
	if (m_pGridMap->IsOccupied(m_pGridMap->GetGridIndex({ m_TargetPosition.x + 1, m_TargetPosition.y, m_TargetPosition.z }))) return;
	m_TargetPosition.x += 1;
	m_MoveTimer = m_MoveSpeed;
}

void GridMovementComponent::MoveWest()
{
	if (IsMoving()) return;
	if (m_pGridMap->IsOccupied(m_pGridMap->GetGridIndex({ m_TargetPosition.x - 1, m_TargetPosition.y, m_TargetPosition.z }))) return;
	m_TargetPosition.x -= 1;
	m_MoveTimer = m_MoveSpeed;
}

void GridMovementComponent::Initialize(const SceneContext&)
{
	// Find the grid map prefab
	m_pGridMap = dynamic_cast<GridMap*>(GetGameObject()->GetScene()->GetGameObjectWithTag(L"GridMap"));
	m_CurrentPosition = GetTransform()->GetPosition();

	m_CurrentGridPosition = m_pGridMap->GetGridIndex(m_CurrentPosition);
	m_TargetPosition = m_CurrentPosition;
}

void GridMovementComponent::Update(const SceneContext& sceneContext)
{
	if (!IsMoving()) return;

	const float dt{ sceneContext.pGameTime->GetElapsed() };
	m_MoveTimer -= dt;
	const float lerp{ 1 - m_MoveTimer / m_MoveSpeed };

	// Calculate the new position
	const float x{ MathHelper::Lerp(m_CurrentPosition.x, m_TargetPosition.x, lerp) };
	const float z{ MathHelper::Lerp(m_CurrentPosition.z, m_TargetPosition.z, lerp) };

	// Update the position
	m_CurrentPosition = { x, m_CurrentPosition.y, z };

	// Update the transform
	GetTransform()->Translate(m_CurrentPosition);

	m_CurrentGridPosition = m_pGridMap->GetGridIndex(m_CurrentPosition);

	// Check if we reached the target position
	if (abs(m_CurrentPosition.x - m_TargetPosition.x) < 0.01f && abs(m_CurrentPosition.z - m_TargetPosition.z) < 0.01f)
	{
		m_CurrentPosition = m_TargetPosition;
		m_TargetPosition = m_CurrentPosition;

		// Update the transform
		GetTransform()->Translate(m_CurrentPosition);
	}
}

bool GridMovementComponent::IsMoving() const
{
	return m_CurrentPosition.x != m_TargetPosition.x || m_CurrentPosition.z != m_TargetPosition.z;
}