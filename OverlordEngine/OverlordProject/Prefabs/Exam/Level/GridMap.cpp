#include "stdafx.h"
#include "GridMap.h"
#include <Prefabs/Exam/Level/Block.h>

const std::wstring GridMap::m_Tag{ L"GridMap" };

GridMap::GridMap(int rows, int cols) :
	m_Rows{ rows },
	m_Cols{ cols }
{}

bool GridMap::IsOccupied(const XMINT2& gridIndex) const
{
	// Check if the grid index is out of bounds
	if (gridIndex.x < 0 || gridIndex.x >= m_Rows || gridIndex.y < 0 || gridIndex.y >= m_Cols)
		return true;

	// Check if the grid index is occupied by a wall
	// Use ranges::any_of to check if any of the grid objects is a wall
	return std::ranges::any_of(m_pGridObjects, [gridIndex](const GridObject* pGridObject)
		{
			return pGridObject->GetPosition().x == gridIndex.x && pGridObject->GetPosition().y == gridIndex.y;
		});
}

XMINT2 GridMap::GetGridIndex(const XMFLOAT3& position) const
{
	// Calculate the grid index based on the position
	const int row{ static_cast<int>(position.x) };
	const int col{ static_cast<int>(position.z) };

	return XMINT2{ row, col };
}

void GridMap::Initialize(const SceneContext&)
{
	SetTag(m_Tag);

	SetUpFloor();
	SetUpWalls();
	SetUpPillars();
}

void GridMap::SetUpFloor() const
{
	// Create a floor plane that is m_Rows by m_Cols in size
	bool useLightGreen = false; // Flag to track the texture index

	for (int row{ 1 }; row < m_Rows - 1; ++row)
	{
		for (int col{ 1 }; col < m_Cols - 1; ++col)
		{
			// For every cell that is not below a wall, create a floor plane
			const std::wstring texturePath{ useLightGreen ? L"Textures/Exam/lime_concrete.png" : L"Textures/Exam/green_concrete.png" };
			auto pBlock{ new Block{ L"Meshes/Exam/UnitTile.ovm", texturePath, false } };
			pBlock->SetPosition(row, col);
			pBlock->SetDimensions(1, 0);
			GetScene()->AddChild(pBlock);

			useLightGreen = !useLightGreen; // Toggle the texture index
		}

		if (m_Cols % 2 == 0) // If the number of columns is even, flip the texture index for the next row
			useLightGreen = !useLightGreen;
	}
}

void GridMap::SetUpWalls()
{
	for (int row{ 0 }; row < m_Rows; ++row)
	{
		for (int col{ 0 }; col < m_Cols; ++col)
		{
			if (row == 0 || row == m_Rows - 1 || col == 0 || col == m_Cols - 1)
			{
				m_pGridObjects.emplace_back(new Block{ L"Meshes/Exam/UnitBox.ovm", L"Textures/Exam/hopper_inside.png", false });
				m_pGridObjects.back()->SetPosition(row, col);
				m_pGridObjects.back()->SetDimensions(1, 1);
				GetScene()->AddChild(m_pGridObjects.back());
			}
		}
	}
}

void GridMap::SetUpPillars()
{
	for (int row{ 2 }; row < m_Rows - 2; row += 2)
	{
		for (int col{ 2 }; col < m_Cols - 2; col += 2)
		{
			m_pGridObjects.emplace_back(new Block{ L"Meshes/Exam/UnitBoxRounded.ovm", L"Textures/Exam/stone.png", false });
			m_pGridObjects.back()->SetPosition(row, col);
			m_pGridObjects.back()->SetDimensions(1, 1);
			GetScene()->AddChild(m_pGridObjects.back());
		}
	}
}
