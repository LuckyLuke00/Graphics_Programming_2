#include "stdafx.h"
#include "GridMovementComponent.h"
#include "Prefabs/Exam/Level/GridMap.h"

GridMovementComponent::GridMovementComponent(float moveTime) :
	m_MoveTime{ moveTime }
{}

void GridMovementComponent::Initialize(const SceneContext&)
{
	// Find the grid map prefab
	m_pGridMap = dynamic_cast<GridMap*>(GetGameObject()->GetScene()->GetGameObjectWithTag(L"GridMap"));
	m_CurrentPosition = GetTransform()->GetPosition();

	m_CurrentGridPosition = m_pGridMap->GetGridIndex(m_CurrentPosition);
	m_TargetPosition = m_CurrentPosition;

	m_pGridObjectOwner = dynamic_cast<GridObject*>(GetGameObject());
}

void GridMovementComponent::MoveNorth()
{
	if (IsMoving()) return;

	const XMINT2 gridIndex{ m_pGridMap->GetGridIndex({ m_TargetPosition.x, m_TargetPosition.y, m_TargetPosition.z + 1 }) };
	if (m_pGridMap->IsOccupiedByPlayer(gridIndex)) return;
	if (m_pGridMap->IsOccupied(gridIndex)) return;

	m_TargetPosition.z += 1;
	m_MoveTimer = m_MoveTime;
	GetTransform()->Rotate(.0f, XM_PI, .0f, false);
}

void GridMovementComponent::MoveSouth()
{
	if (IsMoving()) return;

	const XMINT2 gridIndex{ m_pGridMap->GetGridIndex({ m_TargetPosition.x, m_TargetPosition.y, m_TargetPosition.z - 1 }) };
	if (m_pGridMap->IsOccupiedByPlayer(gridIndex)) return;
	if (m_pGridMap->IsOccupied(gridIndex)) return;

	m_TargetPosition.z -= 1;
	m_MoveTimer = m_MoveTime;
	GetTransform()->Rotate(.0f, .0f, .0f, false);
}

void GridMovementComponent::MoveEast()
{
	if (IsMoving()) return;

	const XMINT2 gridIndex{ m_pGridMap->GetGridIndex({ m_TargetPosition.x + 1, m_TargetPosition.y, m_TargetPosition.z }) };
	if (m_pGridMap->IsOccupiedByPlayer(gridIndex)) return;
	if (m_pGridMap->IsOccupied(gridIndex)) return;

	m_TargetPosition.x += 1;
	m_MoveTimer = m_MoveTime;
	GetTransform()->Rotate(.0f, -XM_PIDIV2, .0f, false);
}

void GridMovementComponent::MoveWest()
{
	if (IsMoving()) return;

	const XMINT2 gridIndex{ m_pGridMap->GetGridIndex({ m_TargetPosition.x - 1, m_TargetPosition.y, m_TargetPosition.z }) };
	if (m_pGridMap->IsOccupiedByPlayer(gridIndex)) return;
	if (m_pGridMap->IsOccupied(gridIndex)) return;

	m_TargetPosition.x -= 1;
	m_MoveTimer = m_MoveTime;
	GetTransform()->Rotate(.0f, XM_PIDIV2, .0f, false);
}

void GridMovementComponent::Update(const SceneContext& sceneContext)
{
	if (!IsMoving())
	{
		// We need to reset, because the transform might have been changed externally
		// Example: When respawning the player
		m_CurrentPosition = GetTransform()->GetPosition();
		m_CurrentGridPosition = m_pGridMap->GetGridIndex(m_CurrentPosition);
		m_TargetPosition = m_CurrentPosition;
		return;
	}

	// when the m_MoveTime is 5f, it should take exactly 5 seconds to reach m_TargetPosition
	const float dt{ sceneContext.pGameTime->GetElapsed() };
	m_MoveTimer -= dt;

	// Calculate the new position
	const float t{ 1.f - (m_MoveTimer / m_MoveTime) };
	const float x{ m_CurrentPosition.x + ((m_TargetPosition.x - m_CurrentPosition.x) * t) };
	const float z{ m_CurrentPosition.z + ((m_TargetPosition.z - m_CurrentPosition.z) * t) };

	// Update the transform
	GetTransform()->Translate(x, m_CurrentPosition.y, z);

	// Check if we reached the target position
	if (m_MoveTimer <= .0f)
	{
		m_CurrentPosition = m_TargetPosition;
		m_CurrentGridPosition = m_pGridMap->GetGridIndex(m_CurrentPosition);
		m_MoveTimer = .0f;

		// Update the transform
		GetTransform()->Translate(m_CurrentPosition);
		if (m_pGridObjectOwner) m_pGridObjectOwner->SetPosition(m_CurrentGridPosition.x, m_CurrentGridPosition.y);
	}
}

bool GridMovementComponent::IsMoving() const
{
	return m_MoveTimer > .0f;
}
