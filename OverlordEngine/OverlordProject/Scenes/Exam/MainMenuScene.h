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
	void LoadSprites();
	UIManager* m_pUIManager{};

	// OnClick functions for the buttons
	void ExitGame();
	void StartGame();
};
