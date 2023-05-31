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

	void Explode();
	void SetExplosionRadius(int explosionRadius) { m_ExplosionRadius = explosionRadius; }
	void SetFuseTime(float fuseTime) { m_FuseTime = fuseTime; }

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext& sceneContext) override;

private:
	PlaceBombComponent* m_pPlaceBombComponent{ nullptr };
	int m_ExplosionRadius{ 1 };

	float m_FuseTime{ 10.f };
	float m_FuseTimer{ 0.f };

	const std::wstring m_ExplostionModel{ L"Meshes/Exam/Explosion.ovm" };
	const std::wstring m_ExplosionTexture{ L"Textures/Exam/Explosion.png" };

	void CreateExplosion(int x, int y);
};
