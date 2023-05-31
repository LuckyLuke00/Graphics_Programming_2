#pragma once
#include "GridObject.h"

class Player;

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

	bool IsOccupied(const XMINT2& gridIndex) const { return IsOccupied(gridIndex.x, gridIndex.y); }
	bool IsOccupied(int row, int col) const;

	bool IsOccupiedByPlayer(const XMINT2& gridIndex) const { return IsOccupiedByPlayer(gridIndex.x, gridIndex.y); }
	bool IsOccupiedByPlayer(int row, int col) const;

	GridObject* GetGridObjectAt(const XMINT2& gridIndex) const { return GetGridObjectAt(gridIndex.x, gridIndex.y); }
	GridObject* GetGridObjectAt(int row, int col) const;

	void AddGridObject(GridObject* pGridObject) { m_pGridObjects.emplace_back(pGridObject); }
	void AddPlayer(Player* pPlayer) { m_pPlayers.emplace_back(pPlayer); }

	void RemoveGridObject(GridObject* pGridObject);
	void RemovePlayer(Player* pPlayer);

	XMINT2 GetGridIndex(const XMFLOAT3& position) const;

protected:
	void Initialize(const SceneContext& sceneContext) override;

private:
	const int m_Rows{ 0 };
	const int m_Cols{ 0 };

	std::vector<GridObject*> m_pGridObjects{};
	std::vector<Player*> m_pPlayers{};

	void SetUpFloor() const;
	void SetUpWalls();
	void SetUpPillars();

	int FindPlayerIndex(int row, int col) const;
	int FindGridObjectIndex(int row, int col) const;
};
