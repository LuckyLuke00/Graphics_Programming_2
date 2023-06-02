#include "stdafx.h"
#include "GridMap.h"
#include "Prefabs/Exam/Level/Block.h"
#include "Prefabs/Exam/Player/Player.h"
#include "Exam/ExamAssets.h"

const std::wstring GridMap::m_Tag{ L"GridMap" };

GridMap::GridMap(int rows, int cols) :
	m_Rows{ rows },
	m_Cols{ cols }
{}

bool GridMap::IsOccupied(int row, int col) const
{
	// Check if the grid index is out of bounds
	if (row < 0 || row >= m_Rows || col < 0 || col >= m_Cols)
		return true;

	return FindGridObjectIndex(row, col) > -1;
}

bool GridMap::IsOccupiedByPlayer(int row, int col) const
{
	// Check if the grid index is out of bounds
	if (row < 0 || row >= m_Rows || col < 0 || col >= m_Cols)
		return false;

	return FindPlayerIndex(row, col) > -1;
}

XMINT2 GridMap::GetGridIndex(const XMFLOAT3& position) const
{
	// Calculate the grid index based on the position
	const int row{ static_cast<int>(position.x) };
	const int col{ static_cast<int>(position.z) };

	return XMINT2{ row, col };
}

void GridMap::RemoveGridObject(GridObject* pGridObject)
{
	const auto it{ std::ranges::find(m_pGridObjects, pGridObject) };
	if (it != m_pGridObjects.end())
		m_pGridObjects.erase(it);
}

void GridMap::RemovePlayer(Player* pPlayer)
{
	const auto it{ std::ranges::find(m_pPlayers, pPlayer) };
	if (it != m_pPlayers.end())
		m_pPlayers.erase(it);
}

GridObject* GridMap::GetGridObjectAt(int row, int col) const
{
	// Check if the grid index is out of bounds
	if (row < 0 || row >= m_Rows || col < 0 || col >= m_Cols)
		return nullptr;

	int index{ FindGridObjectIndex(row, col) };
	if (index > -1)
		return m_pGridObjects[index];

	index = FindPlayerIndex(row, col);
	if (index > -1)
		return m_pPlayers[index];

	return nullptr;
}

void GridMap::Initialize(const SceneContext&)
{
	SetTag(m_Tag);

	SetUpFloor();
	SetUpWalls();
	SetUpPillars();
	SetUpBreakableBlocks();
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
			const std::wstring texturePath{ useLightGreen ? ExamAssets::FloorDarkTexture : ExamAssets::FloorLightTexture };
			auto pBlock{ new Block{ ExamAssets::FloorMesh, texturePath, false } };
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
				AddGridObject(new Block{ ExamAssets::WallMesh, ExamAssets::WallTexture, false });
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
			AddGridObject(new Block{ ExamAssets::PillarMesh, ExamAssets::PillarTexture, false });
			m_pGridObjects.back()->SetPosition(row, col);
			m_pGridObjects.back()->SetDimensions(1, 1);
			GetScene()->AddChild(m_pGridObjects.back());
		}
	}
}

void GridMap::SetUpBreakableBlocks()
{
	const int cornerPadding{ 1 };

	for (int row = 1; row < m_Rows - 1; ++row)
	{
		for (int col = 1; col < m_Cols - 1; ++col)
		{
			if (IsOccupied(row, col)) continue;

			bool isCorner
			{
				// Bottom left corner
				(row == cornerPadding && col == cornerPadding) ||
				(row == cornerPadding + cornerPadding && col == cornerPadding) ||
				(row == cornerPadding && col == cornerPadding + cornerPadding) ||

				// Top left corner
				(row == cornerPadding && col == m_Cols - cornerPadding - 1) ||
				(row == cornerPadding + cornerPadding && col == m_Cols - cornerPadding - 1) ||
				(row == cornerPadding && col == m_Cols - cornerPadding - cornerPadding - 1) ||

				// Bottom right corner
				(row == m_Rows - cornerPadding - 1 && col == cornerPadding) ||
				(row == m_Rows - cornerPadding - cornerPadding - 1 && col == cornerPadding) ||
				(row == m_Rows - cornerPadding - 1 && col == cornerPadding + cornerPadding) ||

				// Top right corner
				(row == m_Rows - cornerPadding - 1 && col == m_Cols - cornerPadding - 1) ||
				(row == m_Rows - cornerPadding - cornerPadding - 1 && col == m_Cols - cornerPadding - 1) ||
				(row == m_Rows - cornerPadding - 1 && col == m_Cols - cornerPadding - cornerPadding - 1)
			};

			if (isCorner) continue;

			Block* pBlock{ new Block{ ExamAssets::BreakableBlockMesh, ExamAssets::BreakableBlockTexture, true } };
			pBlock->SetPosition(row, col);
			pBlock->SetDimensions(1, 1);
			pBlock->OffsetPosition(.0f, -.5f, .0f);
			pBlock->SetScale(.01f, .01f);
			pBlock->MarkForAdd();
		}
	}
}

int GridMap::FindPlayerIndex(int row, int col) const
{
	const auto it{ std::ranges::find_if(m_pPlayers, [row, col](const Player* pPlayer)
				{
			return pPlayer->GetPosition().x == row && pPlayer->GetPosition().y == col;
		}) };

	if (it != m_pPlayers.end()) return static_cast<int>(std::distance(m_pPlayers.begin(), it));

	return -1;
}

int GridMap::FindGridObjectIndex(int row, int col) const
{
	const auto it{ std::ranges::find_if(m_pGridObjects, [row, col](const GridObject* pGridObject)
					{
			return pGridObject->GetPosition().x == row && pGridObject->GetPosition().y == col;
		}) };

	if (it != m_pGridObjects.end()) return static_cast<int>(std::distance(m_pGridObjects.begin(), it));

	return -1;
}
