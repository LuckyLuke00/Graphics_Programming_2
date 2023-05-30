#include "stdafx.h"
#include "Bomb.h"
#include "Explosion.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/Exam/Level/GridMap.h"
#include "Components/PlaceBombComponent.h"

Bomb::Bomb(const std::wstring& model, const std::wstring& texture, PlaceBombComponent* placeBombComponent, int explosionRadius, float fuseTime) :
	m_pPlaceBombComponent{ placeBombComponent },
	m_ExplosionRadius{ explosionRadius },
	m_FuseTime{ fuseTime }
{
	DiffuseMaterial* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	if (!texture.empty()) pMaterial->SetDiffuseTexture(texture);

	AddComponent(new ModelComponent{ model })->SetMaterial(pMaterial);
}

void Bomb::Initialize(const SceneContext&)
{
	SetScale(.01f, .01f);
}

void Bomb::Update(const SceneContext& sceneContext)
{
	m_FuseTimer += sceneContext.pGameTime->GetElapsed();
	if (m_FuseTimer >= m_FuseTime)
	{
		m_FuseTimer = .0f;
		Explode();
		if (m_pPlaceBombComponent) m_pPlaceBombComponent->OnBombExploded(this);
		MarkForDelete();
	}
}

void Bomb::Explode()
{
	for (int i{ 1 }; i <= m_ExplosionRadius; ++i)
	{
		XMINT2 pos{ GetPosition() };
		{
			CreateExplosion(pos.x, pos.y);
		}

		if (!GetGridMap()->IsOccupied(pos.x + 1, pos.y))
		{
			CreateExplosion(pos.x + 1, pos.y);
		}

		if (!GetGridMap()->IsOccupied(pos.x - 1, pos.y))
		{
			CreateExplosion(pos.x - 1, pos.y);
		}

		if (!GetGridMap()->IsOccupied(pos.x, pos.y + 1))
		{
			CreateExplosion(pos.x, pos.y + 1);
		}

		if (!GetGridMap()->IsOccupied(pos.x, pos.y - 1))
		{
			CreateExplosion(pos.x, pos.y - 1);
		}
	}
}

Explosion* Bomb::CreateExplosion(int x, int y)
{
	Explosion* pExplositon{ new Explosion{ m_ExplostionModel, m_ExplosionTexture } };
	pExplositon->SetPosition(x, y);
	pExplositon->MarkForAdd();
	return pExplositon;
}
