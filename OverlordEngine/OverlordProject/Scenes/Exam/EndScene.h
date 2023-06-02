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

	// OnClick functions for the buttons
	void MainMenu();
	void RestartGame();
	void ExitGame();
};
