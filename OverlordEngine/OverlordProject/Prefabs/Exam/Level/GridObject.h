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

	void SetPosition(int row, int col);
	void SetPosition(const XMINT2& position) { SetPosition(position.x, position.y); }
	void SetDimensions(int width, int height);
	void SetScale(float width, float height);
	void SetCollision(bool hasCollision) { m_HasCollision = hasCollision; }

	XMINT2 GetPosition() const { return m_Position; }
	XMINT2 GetDimensions() const { return m_Dimensions; }
	void OffsetPosition(float x = 0, float y = 0, float z = 0);
	bool HasCollision() const { return m_HasCollision; }

	void MarkForAdd() { m_pObjectsToAdd.emplace_back(this); }
	void MarkForDelete() { m_pObjectsToDestroy.emplace_back(this); }

	static void Pause() { m_IsPaused = true; };
	static void UnPause() { m_IsPaused = false; };

	static bool IsPaused() { return m_IsPaused; };

	// Getter for the objects to destroy
	static std::vector<GridObject*>& GetObjectsToDestroy() { return m_pObjectsToDestroy; }
	static std::vector<GridObject*>& GetObjectsToAdd() { return m_pObjectsToAdd; }

protected:
	GridMap* GetGridMap();

private:
	static std::vector<GridObject*> m_pObjectsToDestroy;
	static std::vector<GridObject*> m_pObjectsToAdd;

	bool m_HasCollision{ true };
	static bool m_IsPaused;

	GridMap* m_pGridMap{ nullptr };
	XMFLOAT3 m_Offset{ .0f, .0f, .0f };
	XMINT2 m_Dimensions{ 1, 1 };
	XMINT2 m_Position{ 0, 0 };
};
