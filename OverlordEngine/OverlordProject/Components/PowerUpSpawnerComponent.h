#pragma once

class GridMap;
class GridObject;
class PowerUp;

class PowerUpSpawnerComponent final : public BaseComponent
{
public:
	explicit PowerUpSpawnerComponent() = default;
	~PowerUpSpawnerComponent() override;

	PowerUpSpawnerComponent(const PowerUpSpawnerComponent& other) = delete;
	PowerUpSpawnerComponent(PowerUpSpawnerComponent&& other) noexcept = delete;
	PowerUpSpawnerComponent& operator=(const PowerUpSpawnerComponent& other) = delete;
	PowerUpSpawnerComponent& operator=(PowerUpSpawnerComponent&& other) noexcept = delete;

	// Function that spawns a random power-up on the grid, based on a chance percentage
	void SpawnPowerUp(const XMINT2& gridIndex, float chancePercentage);

protected:
	void Initialize(const SceneContext& sceneContext) override;

private:
	GridMap* m_pGridMap{ nullptr };
	GridObject* m_pGridObjectOwner{ nullptr };

	std::vector<PowerUp*> m_pAvailablePowerUps;
};
