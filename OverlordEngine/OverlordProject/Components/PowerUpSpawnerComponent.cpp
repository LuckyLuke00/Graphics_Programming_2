#include "stdafx.h"
#include "PowerUpSpawnerComponent.h"
#include "Prefabs/Exam/Power-Ups/FirePowerUp.h"

void PowerUpSpawnerComponent::SpawnPowerUp(const XMINT2& gridIndex, float chancePercentage)
{
	if (MathHelper::randF(0.f, 1.f) > chancePercentage) return;

	const int randomIndex{ MathHelper::randI(0, static_cast<int>(m_pAvailablePowerUps.size() - 1)) };
	PowerUp* pPowerUp{ m_pAvailablePowerUps[randomIndex] };

	// Spawn the power-up on the grid
	pPowerUp->SetPosition(gridIndex.x, gridIndex.y);
	pPowerUp->MarkForAdd(false);
}

void PowerUpSpawnerComponent::Initialize(const SceneContext&)
{
	m_pAvailablePowerUps.emplace_back(new FirePowerUp{ L"Meshes/Exam/PowerUp.ovm", L"Textures/Exam/FirePowerUp.png" });
	m_pAvailablePowerUps.back()->SetDimensions(1, 1);
	m_pAvailablePowerUps.back()->OffsetPosition(.0f, -.5f, .0f);
}
