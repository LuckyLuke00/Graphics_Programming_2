#include "stdafx.h"
#include "UIManager.h"
#include "Prefabs/Exam/UI/Buttons/UIButton.h"
#include "Exam/ExamInput.h"

void UIManager::Update(const SceneContext&)
{
	// If there are no buttons, or none of the buttons are selected, return
	if (m_pButtons.empty()) return;

	// For every button, check if it's selected
	if (!m_UsingButtonNavigation)
	{
		for (size_t i{}; i < m_pButtons.size(); ++i)
		{
			// If the button is selected, break out of the loop
			if (m_pButtons[i]->IsSelected())
			{
				m_SelectedButtonIndex = i;
				break;
			}
		}
	}

	if (m_SelectedButtonIndex >= m_pButtons.size()) return;

	UpdateInput();
}

void UIManager::SetFirstSelectedButton(UIButton* pButton)
{
	// Find the index of the button
	const auto it{ std::ranges::find(m_pButtons, pButton) };
	if (it != m_pButtons.end())
	{
		// Set the index of the first selected button
		m_FirstSelectedButtonIndex = std::distance(m_pButtons.begin(), it);
	}
}

void UIManager::AddButton(UIButton* pButton)
{
	// Add the button to the container, but only if it's not already in there
	if (std::ranges::find(m_pButtons, pButton) == m_pButtons.end())
	{
		m_pButtons.emplace_back(pButton);
	}
}

void UIManager::SetSelectedButton(size_t index)
{
	// If the index is out of range, or the index is the same as the current index, return
	if (index >= m_pButtons.size()) return;

	// Deselect the current button
	m_pButtons[m_SelectedButtonIndex]->Deselect();

	// Set the index of the selected button
	m_SelectedButtonIndex = index;
	m_pButtons[m_SelectedButtonIndex]->Select();
}

void UIManager::SetSelectedButton(UIButton* pButton)
{
	if (!pButton) return;

	// Find the index of the button
	const auto it{ std::ranges::find(m_pButtons, pButton) };
	if (it != m_pButtons.end())
	{
		// De-select the current button
		m_pButtons[m_SelectedButtonIndex]->Deselect();

		// Set the index of the selected button
		m_SelectedButtonIndex = std::distance(m_pButtons.begin(), it);
		m_pButtons[m_SelectedButtonIndex]->Select();
	}
}

void UIManager::EnableInput() const
{
	using namespace ExamInput;
	using enum InputActions;

	// Store the pInput
	InputManager* pInput{ GetScene()->GetSceneContext().pInput };

	InputAction submit{ InputAction{ static_cast<int>(Submit), InputState::pressed, VK_RETURN, VK_LBUTTON, XINPUT_GAMEPAD_A } };
	pInput->AddInputAction(submit);

	InputAction up{ InputAction{ static_cast<int>(Up), InputState::pressed, 'W', VK_UP, XINPUT_GAMEPAD_DPAD_UP } };
	pInput->AddInputAction(up);

	InputAction down{ InputAction{ static_cast<int>(Down), InputState::pressed, 'S', VK_DOWN, XINPUT_GAMEPAD_DPAD_DOWN } };
	pInput->AddInputAction(down);

	InputAction left{ InputAction{ static_cast<int>(Left), InputState::pressed, 'A', VK_LEFT, XINPUT_GAMEPAD_DPAD_LEFT } };
	pInput->AddInputAction(left);

	InputAction right{ InputAction{ static_cast<int>(Right), InputState::pressed, 'D', VK_RIGHT, XINPUT_GAMEPAD_DPAD_RIGHT } };
	pInput->AddInputAction(right);
}

void UIManager::UpdateInput()
{
	using namespace ExamInput;
	using enum InputActions;

	const InputManager* pInput{ GetScene()->GetSceneContext().pInput };

	if (pInput->IsActionTriggered(static_cast<int>(Submit)))
	{
		if (!m_pButtons[m_SelectedButtonIndex]->IsSelected()) return;

		// Prevents the button from being clicked when the mouse is not hovering over the button
		if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON) && !m_pButtons[m_SelectedButtonIndex]->IsMouseHovering())
		{
			UIButton::SetIsUsingNavigation(false);
			return;
		}

		m_pButtons[m_SelectedButtonIndex]->OnClick();
		return;
	}

	if (pInput->IsActionTriggered(static_cast<int>(Up)))
	{
		m_UsingButtonNavigation = true;
		if (!m_pButtons[m_SelectedButtonIndex]->IsSelected())
		{
			SetSelectedButton(m_FirstSelectedButtonIndex);
			return;
		}

		SetSelectedButton(m_pButtons[m_SelectedButtonIndex]->GetUpButton());

		return;
	}

	if (pInput->IsActionTriggered(static_cast<int>(Down)))
	{
		m_UsingButtonNavigation = true;
		if (!m_pButtons[m_SelectedButtonIndex]->IsSelected())
		{
			SetSelectedButton(m_FirstSelectedButtonIndex);
			return;
		}

		SetSelectedButton(m_pButtons[m_SelectedButtonIndex]->GetDownButton());

		return;
	}

	if (pInput->IsActionTriggered(static_cast<int>(Left)))
	{
		m_UsingButtonNavigation = true;
		if (!m_pButtons[m_SelectedButtonIndex]->IsSelected())
		{
			SetSelectedButton(m_FirstSelectedButtonIndex);
			return;
		}

		SetSelectedButton(m_pButtons[m_SelectedButtonIndex]->GetLeftButton());

		return;
	}

	if (pInput->IsActionTriggered(static_cast<int>(Right)))
	{
		m_UsingButtonNavigation = true;
		if (!m_pButtons[m_SelectedButtonIndex]->IsSelected())
		{
			SetSelectedButton(m_FirstSelectedButtonIndex);
			return;
		}

		SetSelectedButton(m_pButtons[m_SelectedButtonIndex]->GetRightButton());

		return;
	}
}
