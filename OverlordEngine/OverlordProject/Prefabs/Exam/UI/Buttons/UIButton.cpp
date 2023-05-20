#include "stdafx.h"
#include "UIButton.h"

bool UIButton::m_IsUsingNavigation{ false };

UIButton::UIButton(SpriteFont* pFont, const std::wstring& text, DirectX::XMFLOAT2 position) :
	m_pFont{ pFont },
	m_Text{ text },
	m_Position{ position }
{}

void UIButton::SetNavigationButtons(UIButton* pUp, UIButton* pDown, UIButton* pLeft, UIButton* pRight)
{
	m_pNavigationButtons.emplace_back(pUp);
	m_pNavigationButtons.emplace_back(pDown);
	m_pNavigationButtons.emplace_back(pLeft);
	m_pNavigationButtons.emplace_back(pRight);
}

bool UIButton::IsMouseHovering() const
{
	const XMFLOAT2 mousePos
	{
		static_cast<float>(InputManager::GetMousePosition().x),
		static_cast<float>(InputManager::GetMousePosition().y)
	};

	const auto size{ SpriteFont::MeasureString(m_Text, m_pFont) };

	return mousePos.x >= m_Position.x &&
		mousePos.x <= m_Position.x + size.x &&
		mousePos.y >= m_Position.y &&
		mousePos.y <= m_Position.y + size.y;
}

void UIButton::Update(const SceneContext&)
{
	UpdateButtonColor();
}

void UIButton::CalculateMouseHovering()
{
	if (IsMouseHovering())
	{
		m_IsSelected = true;
		m_IsUsingNavigation = false;
	}
	else if (m_IsSelected && !m_IsUsingNavigation)
	{
		m_IsSelected = false;
	}
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
