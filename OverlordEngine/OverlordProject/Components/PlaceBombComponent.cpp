#include "stdafx.h"
#include "PlaceBombComponent.h"
#include "Prefabs/Exam/Bomb/Explosion.h"

PlaceBombComponent::PlaceBombComponent(int maxBombs, int explosionRadius) :
	m_ExplosionRadius{ explosionRadius },
	m_MaxBombs{ maxBombs }
{
	m_pLiveBombs.reserve(maxBombs);
}

void PlaceBombComponent::PlaceBomb()
{
	if (m_MaxBombs <= m_pLiveBombs.size()) return;
	// If the current position already has a bomb, don't place another one

	const auto& position{ m_pGridObjectOwner->GetPosition() };
	// See if any of the live bombs are at the same position
	if (const auto& it
		{
			std::ranges::find_if(m_pLiveBombs, [position](const Bomb* pBomb)
			{
				return pBomb->GetPosition().x == position.x && pBomb->GetPosition().y == position.y;
			})
		};
		it != m_pLiveBombs.end()) return;

	Bomb* pBomb{ new Bomb{ L"Meshes/Exam/Bomb.ovm", L"Textures/Exam/Bomb.png", this } };
	pBomb->SetPosition(position.x, position.y);
	pBomb->MarkForAdd();

	m_pLiveBombs.emplace_back(pBomb);
}

void PlaceBombComponent::OnBombExploded(Bomb* pBomb)
{
	const auto& it{ std::ranges::find(m_pLiveBombs, pBomb) };
	if (it != m_pLiveBombs.end())
	{
		m_pLiveBombs.erase(it);
	}
}

void PlaceBombComponent::Initialize(const SceneContext&)
{
	m_pGridMap = dynamic_cast<GridMap*>(GetGameObject()->GetScene()->GetGameObjectWithTag(L"GridMap"));

	if (m_pGridMap == nullptr)
	{
		Logger::LogError(L"Bomb::Initialize() > Failed to find GridMap");
		return;
	}

	m_pGridObjectOwner = dynamic_cast<GridObject*>(GetGameObject());

	if (m_pGridObjectOwner == nullptr)
	{
		Logger::LogError(L"PlaceBombComponent::PlaceBombComponent() > Failed to find GridObject");
		return;
	}
}
