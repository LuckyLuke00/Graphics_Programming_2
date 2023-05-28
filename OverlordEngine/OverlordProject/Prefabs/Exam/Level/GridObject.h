#pragma once
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
	void OffsetPosition(float x = 0, float y = 0, float z = 0) const;

private:
	XMINT2 m_Dimensions{ 1, 1 };
	XMINT2 m_Position{ 0, 0 };
};
