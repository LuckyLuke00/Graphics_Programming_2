#pragma once

class GridMap;
class CountdownTimer;
class UIManager;

class LevelScene final : public GameScene
{
public:
	LevelScene() : GameScene(L"LevelScene") {}
	~LevelScene() override;

	LevelScene(const LevelScene& other) = delete;
	LevelScene(LevelScene&& other) noexcept = delete;
	LevelScene& operator=(const LevelScene& other) = delete;
	LevelScene& operator=(LevelScene&& other) noexcept = delete;

	void TogglePause();

protected:
	void Initialize() override;
	void Update() override;
	void OnSceneActivated() override;

private:
	UIManager* m_pUIManager{};
	GameObject* m_pPauseMenu{ nullptr };
	GridMap* m_pGridMap{ nullptr };
	CountdownTimer* m_pCountdownTimer{ nullptr };

	bool m_Paused{ false };
	int m_MaxPlayers{ 4 };

	void SetupPlayer(int playerIndex) const;
	void SetUpCamera();
	void CreateGroundPlane();

	void RemoveGridObjects();
	void AddGridObjects();

	bool HasGameEnded() const;
	void CreatePauseMenu();

	void Reset();
};
