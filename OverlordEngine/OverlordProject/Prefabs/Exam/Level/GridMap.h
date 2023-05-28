#pragma once
#include "GridObject.h"

class GridMap : public GameObject
{
public:
	static const std::wstring m_Tag;

	explicit GridMap(int rows, int cols);
	~GridMap() override = default;

	GridMap(const GridMap& other) = delete;
	GridMap(GridMap&& other) noexcept = delete;
	GridMap& operator=(const GridMap& other) = delete;
	GridMap& operator=(GridMap&& other) noexcept = delete;

	bool IsOccupied(const XMINT2& gridIndex) const;
	XMINT2 GetGridIndex(const XMFLOAT3& position) const;

	void AddGridObject(GridObject* pGridObject) { m_pGridObjects.emplace_back(pGridObject); }

protected:
	void Initialize(const SceneContext& sceneContext) override;

private:
	const int m_Rows{ 0 };
	const int m_Cols{ 0 };

	std::vector<GridObject*> m_pGridObjects{};

	void SetUpFloor() const;
	void SetUpWalls();
	void SetUpPillars();
};
