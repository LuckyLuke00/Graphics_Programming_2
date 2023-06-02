#include "stdafx.h"
#include "PowerUpSpawnerComponent.h"
#include "Prefabs/Exam/Power-Ups/FirePowerUp.h"
#include "Prefabs/Exam/Power-Ups/BombUpPowerUp.h"
#include "Exam/ExamAssets.h"

void PowerUpSpawnerComponent::SpawnPowerUp(const XMINT2& gridIndex, float chancePercentage)
{
	if (MathHelper::randF(0.f, 1.f) > chancePercentage) return;

	const int randomIndex{ MathHelper::randI(0, static_cast<int>(m_pAvailablePowerUps.size() - 1)) };
	PowerUp* pPowerUp{ m_pAvailablePowerUps[randomIndex] };

	// Spawn the power-up on the grid
	pPowerUp->SetPosition(gridIndex.x, gridIndex.y);
	pPowerUp->SetCollision(false);
	pPowerUp->MarkForAdd();
}

void PowerUpSpawnerComponent::Initialize(const SceneContext&)
{
	m_pAvailablePowerUps.emplace_back(new FirePowerUp{ ExamAssets::PowerUpMesh, ExamAssets::FirePowerUpTexture });
	m_pAvailablePowerUps.back()->SetDimensions(1, 1);
	m_pAvailablePowerUps.back()->OffsetPosition(.0f, -.5f, .0f);

	m_pAvailablePowerUps.emplace_back(new BombUpPowerUp{ ExamAssets::PowerUpMesh, ExamAssets::BombUpPowerUpTexture });
	m_pAvailablePowerUps.back()->SetDimensions(1, 1);
	m_pAvailablePowerUps.back()->OffsetPosition(.0f, -.5f, .0f);
}
