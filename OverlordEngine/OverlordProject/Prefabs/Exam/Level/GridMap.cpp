#include "stdafx.h"
#include "GridMap.h"
#include <Prefabs/Exam/Level/Block.h>

GridMap::GridMap(int rows, int cols, int cellSize) :
	m_Rows{ rows },
	m_Cols{ cols },
	m_CellSize{ cellSize }
{}

void GridMap::Initialize(const SceneContext&)
{
	SetUpFloor();
	SetUpWalls();
	SetUpPillars();
}

void GridMap::SetUpFloor()
{
	// Create a floor plane that is m_Rows by m_Cols in size
	bool useLightGreen = false; // Flag to track the texture index

	for (int row{ 1 }; row < m_Rows - 1; ++row)
	{
		for (int col{ 1 }; col < m_Cols - 1; ++col)
		{
			// For every cell that is not below a wall, create a floor plane
			const std::wstring texturePath = useLightGreen ? L"Textures/BoxLightGreen.png" : L"Textures/BoxDarkGreen.png";
			m_pGridObjects.emplace_back(new Block{ L"Meshes/UnitPlane.ovm", texturePath, false });
			m_pGridObjects.back()->SetScale(.1f, .1f);
			m_pGridObjects.back()->SetPosition(row, col);
			m_pGridObjects.back()->SetDimensions(1, 0);
			GetScene()->AddChild(m_pGridObjects.back());

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
				m_pGridObjects.emplace_back(new Block{ L"Meshes/UnitBox.ovm", L"Textures/BoxMetal.png", false });
				m_pGridObjects.back()->SetScale(.5f, .5f);
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
			m_pGridObjects.emplace_back(new Block{ L"Meshes/UnitBox.ovm", L"Textures/BoxLightMetal.png", false });
			m_pGridObjects.back()->SetScale(.5f, .5f);
			m_pGridObjects.back()->SetPosition(row, col);
			m_pGridObjects.back()->SetDimensions(1, 1);
			GetScene()->AddChild(m_pGridObjects.back());
		}
	}
}
