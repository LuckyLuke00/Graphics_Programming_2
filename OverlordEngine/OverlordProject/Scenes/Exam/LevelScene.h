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

protected:
	void Initialize() override;
	void Update() override;
	void OnSceneActivated() override;

private:
	PostBloom* m_pPostBloom{ nullptr };
	UIManager* m_pUIManager{};
	GameObject* m_pPauseMenu{ nullptr };
	GridMap* m_pGridMap{ nullptr };
	CountdownTimer* m_pCountdownTimer{ nullptr };
	std::vector<UIButton*> m_pPauseButtons{};

	std::wstring m_SceneToLoad{};
	bool m_UnloadScene{ false };

	bool m_Paused{ false };
	int m_MaxPlayers{ 4 };

	void SetupPlayer(int playerIndex) const;
	void SetUpCamera();
	void CreateGroundPlane();

	void RemoveGridObjects();
	void AddGridObjects();

	bool HasGameEnded() const;
	void CreatePauseMenu();
	void SetUpLevel();
	void ClearPauseMenu();

	void Reset();
	UIButton* CreatePauseButtons(const std::wstring& text, const XMFLOAT2& pos) const;
};
