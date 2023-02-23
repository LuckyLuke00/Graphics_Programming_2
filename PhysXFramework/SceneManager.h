#pragma once

class GameScene;
class SceneContext;
class DebugRenderer;

class SceneManager final
{
public:
	SceneManager();
	~SceneManager();
	SceneManager(const SceneManager& t) = delete;
	SceneManager& operator=(const SceneManager& t) = delete;

	void AddGameScene(GameScene* pScene);
	void RemoveGameScene(GameScene* pScene);
	void SetActiveGameScene(const std::wstring& sceneName);
	void NextScene();
	void PreviousScene();
	bool IsInputEnabled() { return m_InputEnabled; }

	void Initialize();
	void Update();
	void Draw();
	void Destroy();
	void WindowStateChanged(bool active);
	void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam);
	void ToggleDebugRenderer();
private:
	std::vector<GameScene*> m_Scenes;
	bool m_IsInitialized;
	GameScene* m_pActiveScene, * m_pNewActiveScene;
	bool m_InputEnabled;
	std::shared_ptr<DebugRenderer>m_pDebugRenderer;
};
