#pragma once
#include "GridObject.h"

class GridMap : public GameObject
{
public:
	explicit GridMap(int rows, int cols, int cellSize = 1);
	~GridMap() override = default;

	GridMap(const GridMap& other) = delete;
	GridMap(GridMap&& other) noexcept = delete;
	GridMap& operator=(const GridMap& other) = delete;
	GridMap& operator=(GridMap&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;

private:
	const int m_Rows{ 0 };
	const int m_Cols{ 0 };
	const int m_CellSize{ 1 };

	std::vector<GridObject*> m_pGridObjects{};

	void SetUpFloor();
	void SetUpWalls();
	void SetUpPillars();
};
