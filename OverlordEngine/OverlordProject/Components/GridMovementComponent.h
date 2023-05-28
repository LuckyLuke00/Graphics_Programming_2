#pragma once
#include <Prefabs/Exam/Level/GridMap.h>

class GridMovementComponent : public BaseComponent
{
public:
	explicit GridMovementComponent(float moveSpeed = 1.f);
	~GridMovementComponent() override = default;

	GridMovementComponent(const GridMovementComponent& other) = delete;
	GridMovementComponent(GridMovementComponent&& other) noexcept = delete;
	GridMovementComponent& operator=(const GridMovementComponent& other) = delete;
	GridMovementComponent& operator=(GridMovementComponent&& other) noexcept = delete;

	void MoveNorth();
	void MoveSouth();
	void MoveEast();
	void MoveWest();

	bool IsMoving() const;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	GridMap* m_pGridMap{ nullptr };
	GridObject* m_pGridObjectOwner{ nullptr };

	XMFLOAT3 m_CurrentPosition{ 0, 0, 0 };
	XMFLOAT3 m_TargetPosition{ 0, 0, 0 };
	XMINT2 m_CurrentGridPosition{ 0, 0 };

	float m_MoveSpeed{ 1.f };
	float m_MoveTimer{ .0f };
};
