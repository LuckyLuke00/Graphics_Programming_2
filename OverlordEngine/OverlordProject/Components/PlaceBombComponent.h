#pragma once
#include "Prefabs/Exam/Level/GridMap.h"
#include "Prefabs/Exam/Bomb/Bomb.h"

class PlaceBombComponent : public BaseComponent
{
public:
	explicit PlaceBombComponent(int maxBombs = 1, int explosionRadius = 1);
	~PlaceBombComponent() override = default;

	PlaceBombComponent(const PlaceBombComponent& other) = delete;
	PlaceBombComponent(PlaceBombComponent&& other) noexcept = delete;
	PlaceBombComponent& operator=(const PlaceBombComponent& other) = delete;
	PlaceBombComponent& operator=(PlaceBombComponent&& other) noexcept = delete;

	void SetBombTimer(float bombTimer) { m_BombTimerMax = bombTimer; }
	void SetExplosionRadius(int explosionRadius) { m_ExplosionRadius = explosionRadius; }
	void SetMaxBombs(int maxBombs) { m_MaxBombs = maxBombs; m_pLiveBombs.reserve(m_MaxBombs); }

	void SetGridMap(GridMap* pGridMap) { m_pGridMap = pGridMap; }
	void SetGridObjectOwner(GridObject* pGridObjectOwner) { m_pGridObjectOwner = pGridObjectOwner; }

	void PlaceBomb();
	void OnBombExploded(Bomb* pBomb);

protected:
	void Initialize(const SceneContext& sceneContext) override;

private:
	int m_ExplosionRadius{ 1 };
	int m_MaxBombs{ 1 };

	float m_BombTimerMax{ 3.f };
	float m_BombTimer{ .0f };

	GridMap* m_pGridMap{ nullptr };
	GridObject* m_pGridObjectOwner{ nullptr };

	std::vector<Bomb*> m_pLiveBombs;

	Explosion* CreateExplosion(int x, int y);
};
