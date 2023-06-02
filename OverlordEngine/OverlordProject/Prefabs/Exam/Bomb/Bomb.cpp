#include "stdafx.h"
#include "Bomb.h"
#include "Explosion.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/Exam/Level/GridMap.h"
#include "Components/PlaceBombComponent.h"
#include "Prefabs/Exam/Player/Player.h"
#include "Prefabs/Exam/Level/Block.h"
#include "Exam/ExamAssets.h"

Bomb::Bomb(const std::wstring& model, const std::wstring& texture, PlaceBombComponent* placeBombComponent, int explosionRadius, float fuseTime) :
	m_pPlaceBombComponent{ placeBombComponent },
	m_ExplosionRadius{ explosionRadius },
	m_FuseTime{ fuseTime }
{
	DiffuseMaterial* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	if (!texture.empty()) pMaterial->SetDiffuseTexture(texture);

	m_pModelComponent = AddComponent(new ModelComponent{ model });
	m_pModelComponent->SetMaterial(pMaterial);
}

void Bomb::Initialize(const SceneContext&)
{
	SetScale(.01f, .01f);
}

void Bomb::OnSceneAttach(GameScene*)
{
	m_pModelAnimator = m_pModelComponent->GetAnimator();
	m_pModelAnimator->SetAnimationSpeed(1.f);
	m_pModelAnimator->SetAnimation(0);
}

void Bomb::Update(const SceneContext& sceneContext)
{
	if (!m_pModelAnimator->IsPlaying())
	{
		m_pModelAnimator->Play();
	}

	m_FuseTimer += sceneContext.pGameTime->GetElapsed();
	if (m_FuseTimer >= m_FuseTime)
	{
		m_FuseTimer = .0f;
		Explode();
	}
}

void Bomb::Explode()
{
	for (int i{ 1 }; i <= m_ExplosionRadius; ++i)
	{
		const XMINT2& pos{ GetPosition() };

		CreateExplosion(pos.x, pos.y);
		CreateExplosion(pos.x + 1, pos.y);
		CreateExplosion(pos.x - 1, pos.y);
		CreateExplosion(pos.x, pos.y + 1);
		CreateExplosion(pos.x, pos.y - 1);
	}

	if (m_pPlaceBombComponent) m_pPlaceBombComponent->OnBombExploded(this);
	MarkForDelete();
}

void Bomb::CreateExplosion(int x, int y)
{
	// If the position is the same as the bomb's position, still create an explosion
	// This is to make sure the bomb itself is destroyed
	const XMINT2& pos{ GetPosition() };
	bool isBombPosition{ x == pos.x && y == pos.y };
	auto pGridObject{ GetGridMap()->GetGridObjectAt(x, y) };

	if (!isBombPosition && pGridObject)
	{
		const Block* pBlock{ dynamic_cast<Block*>(pGridObject) };
		const Bomb* pBomb{ dynamic_cast<Bomb*>(pGridObject) };
		const Player* pPlayer{ dynamic_cast<Player*>(pGridObject) };

		if (!pBlock && !pBomb && !pPlayer) return;

		if (pBlock && !pBlock->IsBreakable()) return;
	}

	Explosion* pExplositon{ new Explosion{ ExamAssets::ExplosionMesh, ExamAssets::ExplosionTexture } };
	pExplositon->SetPosition(x, y);
	pExplositon->OffsetPosition(.0f, -.5f);
	pExplositon->MarkForAdd(false);
}
