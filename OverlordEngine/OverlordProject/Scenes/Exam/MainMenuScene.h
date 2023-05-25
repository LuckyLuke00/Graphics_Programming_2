#pragma once

class UIManager;

class MainMenuScene final : public GameScene
{
public:
	MainMenuScene() : GameScene(L"MainMenuScene") {}
	~MainMenuScene() override = default;

	MainMenuScene(const MainMenuScene& other) = delete;
	MainMenuScene(MainMenuScene&& other) noexcept = delete;
	MainMenuScene& operator=(const MainMenuScene& other) = delete;
	MainMenuScene& operator=(MainMenuScene&& other) noexcept = delete;

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
