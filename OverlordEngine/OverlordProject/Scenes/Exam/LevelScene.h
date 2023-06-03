#pragma once

class CountdownTimer;
class GridMap;
class PostBloom;
class UIButton;
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
	static bool HasGameStarted() { return m_GameStarted; }

protected:
	void Initialize() override;
	void Update() override;
	void OnSceneActivated() override;

private:
	FMOD::Sound* m_pBattleStartSound{ nullptr };
	PostBloom* m_pPostBloom{ nullptr };
	UIManager* m_pUIManager{};
	GameObject* m_pPauseMenu{ nullptr };
	GridMap* m_pGridMap{ nullptr };
	CountdownTimer* m_pCountdownTimer{ nullptr };
	std::vector<UIButton*> m_pPauseButtons{};

	static bool m_GameStarted;
	bool m_Paused{ false };
	int m_MaxPlayers{ 4 };
	int m_PlayerCount{ 0 };

	bool WaitForPlayers();
	void SetupPlayer(int playerIndex) const;
	void SetUpCamera();
	void CreateGroundPlane();
	void SetupTimer();

	void RemoveGridObjects();
	void AddGridObjects();

	bool HasGameEnded() const;
	void CreatePauseMenu();
	void SetUpLevel();
	void ClearPauseMenu();

	void Reset();
	UIButton* CreatePauseButtons(const std::wstring& text, const XMFLOAT2& pos) const;
};
