#pragma once

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
	void OnSceneActivated() override;
private:
	void SetUpCamera();
	void CreateGroundPlane();
};
