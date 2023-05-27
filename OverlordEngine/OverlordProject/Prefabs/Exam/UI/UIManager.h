#pragma once

class UIButton;

class UIManager final : public GameObject
{
public:
	UIManager() = default;
	~UIManager() override = default;
	UIManager(UIManager& other) = delete;
	UIManager(UIManager&& other) noexcept = delete;
	UIManager& operator=(UIManager& other) = delete;
	UIManager& operator=(UIManager&& other) noexcept = delete;

	void AddButton(UIButton* pButton);
	void SetFirstSelectedButton(UIButton* pButton);
	void SetSelectedButton(size_t index);
	void SetSelectedButton(UIButton* pButton);

	void EnableInput() const;

protected:
	void Update(const SceneContext&) override;

private:

	// Container for all the buttons
	bool m_UsingButtonNavigation{ false };
	size_t m_FirstSelectedButtonIndex{ 0 };
	size_t m_SelectedButtonIndex{ 0 };
	std::vector<UIButton*> m_pButtons;

	void UpdateInput();
};
