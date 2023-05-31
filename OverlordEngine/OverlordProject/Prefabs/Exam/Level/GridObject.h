#pragma once

class GridMap;

class GridObject : public GameObject
{
public:
	explicit GridObject() = default;
	~GridObject() override = default;

	GridObject(const GridObject& other) = delete;
	GridObject(GridObject&& other) noexcept = delete;
	GridObject& operator=(const GridObject& other) = delete;
	GridObject& operator=(GridObject&& other) noexcept = delete;

	virtual void SetPosition(int row, int col);
	virtual void SetDimensions(int width, int height);
	virtual void SetScale(float width, float height);

	XMINT2 GetPosition() const { return m_Position; }
	XMINT2 GetDimensions() const { return m_Dimensions; }
	void OffsetPosition(float x = 0, float y = 0, float z = 0);

	void MarkForAdd(bool addToGridobject = true) { m_pObjectsToAdd.emplace_back(std::make_pair(this, addToGridobject)); }
	void MarkForDelete() { m_pObjectsToDestroy.emplace_back(this); }

	// Getter for the objects to destroy
	static std::vector<GridObject*>& GetObjectsToDestroy() { return m_pObjectsToDestroy; }
	static std::vector<std::pair<GridObject*, bool>>& GetObjectsToAdd() { return m_pObjectsToAdd; }

protected:
	GridMap* GetGridMap();

private:
	static std::vector<GridObject*> m_pObjectsToDestroy;
	static std::vector<std::pair<GridObject*, bool>> m_pObjectsToAdd;

	GridMap* m_pGridMap{ nullptr };
	XMFLOAT3 m_Offset{ .0f, .0f, .0f };
	XMINT2 m_Dimensions{ 1, 1 };
	XMINT2 m_Position{ 0, 0 };
};
