#pragma once

class UIManager;

class EndScene final : public GameScene
{
public:
	EndScene() : GameScene(L"EndScene") {}
	~EndScene() override = default;

	EndScene(const EndScene& other) = delete;
	EndScene(EndScene&& other) noexcept = delete;
	EndScene& operator=(const EndScene& other) = delete;
	EndScene& operator=(EndScene&& other) noexcept = delete;

protected:
	void Initialize() override;

private:

	enum class Menus
	{
		MainMenu,
		None
	};

	// Function that loads and opens the specified menu
	void LoadMenu(const Menus& menu);
	void LoadSprites();

	// Container that holds all the menu's
	std::map<Menus, GameObject*> m_pMenuBackgroundSprites{};

	// Variable that stores the current menu
	Menus m_CurrentMenu{ Menus::None };
	UIManager* m_pUIManager{ };

	// OnClick functions for the buttons
	void ExitGame();
	void StartGame();
};
