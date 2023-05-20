#include "stdafx.h"
#include "UIButton.h"

UIButton::UIButton(SpriteFont* pFont, const std::wstring& text, DirectX::XMFLOAT2 position) :
	m_pFont{ pFont },
	m_Text{ text },
	m_Position{ position }
{}

void UIButton::Update(const SceneContext&)
{
	UpdateButtonColor();
}

void UIButton::CalculateMouseHovering()
{
	// See if the mouse is hovering over the button
	const auto mousePos{ InputManager::GetMousePosition() };
	const auto position{ DirectX::XMFLOAT2{ static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) } };

	const auto size{ SpriteFont::MeasureString(m_Text, m_pFont) };

	m_IsSelected =
		position.x >= m_Position.x && position.x <= m_Position.x + size.x &&
		position.y >= m_Position.y && position.y <= m_Position.y + size.y;
}

void UIButton::UpdateButtonColor()
{
	CalculateMouseHovering();

	const XMFLOAT4 color{ m_IsSelected ? m_SelectedColor : m_NormalColor };

	m_CurrentColor = color;
	TextRenderer::Get()->DrawText(m_pFont, m_Text, m_Position, m_CurrentColor);
}

void UIButton::OnClick() const
{
	if (m_OnClickFunction) m_OnClickFunction();
}
