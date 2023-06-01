#pragma once
#include "Prefabs/Exam/Level/GridMap.h"
#include <array>

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
	GridMap* m_pGridMap{ nullptr };
	int m_MaxPlayers{ 4 };

	std::array<XMINT2, 4> m_PlayerSpawnPoints
	{
		XMINT2{ 1, 1 },
		XMINT2{ 17, 11 },
		XMINT2{ 17, 1 },
		XMINT2{ 1, 11 }
	};

	void SetupPlayer(int xPos, int yPos) const;
	void SetUpCamera();
	void CreateGroundPlane();
};
