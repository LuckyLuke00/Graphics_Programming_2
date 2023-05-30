#pragma once
#include "Prefabs/Exam/Level/GridObject.h"

class GridMap;
class PlaceBombComponent;
class Explosion;

class Bomb : public GridObject
{
public:
	explicit Bomb(const std::wstring& model, const std::wstring& texture, PlaceBombComponent* placeBombComponent, int explosionRadius = 1, float fuseTime = 3.f);
	~Bomb() override = default;

	Bomb(const Bomb& other) = delete;
	Bomb(Bomb&& other) noexcept = delete;
	Bomb& operator=(const Bomb& other) = delete;
	Bomb& operator=(Bomb&& other) noexcept = delete;

	void SetFuseTime(float fuseTime) { m_FuseTime = fuseTime; }
	void SetExplosionRadius(int explosionRadius) { m_ExplosionRadius = explosionRadius; }

	bool ShouldExplode() const { return m_FuseTimer >= m_FuseTime; }

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext& sceneContext) override;

private:
	PlaceBombComponent* m_pPlaceBombComponent{ nullptr };
	int m_ExplosionRadius{ 1 };

	float m_FuseTime{ 3.f };
	float m_FuseTimer{ 0.f };

	const std::wstring m_ExplostionModel{ L"Meshes/Exam/Explosion.ovm" };
	const std::wstring m_ExplosionTexture{ L"Textures/Exam/Explosion.png" };

	void Explode();
	Explosion* CreateExplosion(int x, int y);
};
