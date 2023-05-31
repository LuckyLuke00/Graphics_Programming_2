#pragma once
#include <Prefabs/Exam/Level/GridMap.h>

class LevelScene final : public GameScene
{
public:
	LevelScene() : GameScene(L"LevelScene") {}
	~LevelScene() override = default;

	LevelScene(const LevelScene& other) = delete;
	LevelScene(LevelScene&& other) noexcept = delete;
	LevelScene& operator=(const LevelScene& other) = delete;
	LevelScene& operator=(LevelScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void OnSceneActivated() override;

private:
	void SetupPlayer(int xPos, int yPos);
	void SetUpCamera();
	void CreateGroundPlane();

	GridMap* m_pGridMap{ nullptr };
};
