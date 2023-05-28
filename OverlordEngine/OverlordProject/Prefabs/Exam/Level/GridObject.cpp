#include "stdafx.h"
#include "GridObject.h"

void GridObject::SetPosition(int row, int col)
{
	m_Position.x = row;
	m_Position.y = col;

	// Translate the position to world space, with half the dimensions as offset
	const float posX{ m_Position.x + m_Dimensions.x * .5f };
	const float posY{ m_Dimensions.y * .5f };
	const float posZ{ m_Position.y + m_Dimensions.x * .5f };

	GetTransform()->Translate(posX, posY, posZ);
}

void GridObject::SetDimensions(int width, int height)
{
	m_Dimensions.x = width;
	m_Dimensions.y = height;

	// Get the scale of the object
	XMFLOAT3 scale{ GetTransform()->GetScale() };

	// Prevent z-fighting when scaling to 0
	const float scaleX{ std::max(.001f, std::max(scale.x, scale.z) * static_cast<float>(m_Dimensions.x)) };
	const float scaleY{ std::max(.001f, scale.y * static_cast<float>(m_Dimensions.y)) };

	// Scale the object to the given dimensions
	GetTransform()->Scale(scaleX, scaleY, scaleX);

	// Translate the object to the correct position
	SetPosition(m_Position.x, m_Position.y);
}

void GridObject::SetScale(float width, float height)
{
	// Scale the object to the given dimensions
	GetTransform()->Scale(width, height, width);

	// Translate the object to the correct position
	SetPosition(m_Position.x, m_Position.y);
}

void GridObject::OffsetPosition(float x, float y, float z)
{
	const auto& pos{ GetTransform()->GetPosition() };

	// Translate the object to the correct position
	GetTransform()->Translate(pos.x + x, pos.y + y, pos.z + z);
}
