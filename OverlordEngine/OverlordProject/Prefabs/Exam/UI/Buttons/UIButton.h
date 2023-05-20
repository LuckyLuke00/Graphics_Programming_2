#pragma once

class UIButton : public GameObject
{
public:
	UIButton(SpriteFont* pFont, const std::wstring& text, DirectX::XMFLOAT2 position = { .0f, .0f });
	~UIButton() override = default;
	UIButton(UIButton& other) = delete;
	UIButton(UIButton&& other) noexcept = delete;
	UIButton& operator=(UIButton& other) = delete;
	UIButton& operator=(UIButton&& other) noexcept = delete;

	bool IsSelected() const { return m_IsSelected; }
	void Select() { m_IsSelected = true; }
	void Deselect() { m_IsSelected = false; }

	void OnClick() const;

	void SetNormalColor(const DirectX::XMFLOAT4& color) { m_NormalColor = color; }
	void SetOnClickFunction(const std::function<void()>& onClickFunction) { m_OnClickFunction = onClickFunction; }
	void SetPosition(const DirectX::XMFLOAT2& position) { m_Position = position; }
	void SetSelectedColor(const DirectX::XMFLOAT4& color) { m_SelectedColor = color; }
	void SetText(const std::wstring_view& text) { m_Text = text; }

protected:
	void Update(const SceneContext&) override;

private:
	SpriteFont* m_pFont{ nullptr };
	std::wstring m_Text{ L"Button Text" };
	DirectX::XMFLOAT2 m_Position{ .0f, .0f };
	DirectX::XMFLOAT4 m_CurrentColor{ Colors::Gray };
	DirectX::XMFLOAT4 m_NormalColor{ Colors::Gray };
	DirectX::XMFLOAT4 m_SelectedColor{ Colors::White };

	// Function that will be called when the button is clicked
	std::function<void()> m_OnClickFunction{ nullptr };

	// Mouse hovering
	bool m_IsSelected{ false };

	// Function that calculates if the mouse is hovering over the button
	void CalculateMouseHovering();
	void UpdateButtonColor();
};
