#include "stdafx.h"
#include "LevelScene.h"
#include <Prefabs/Exam/Level/GridMap.h>

void LevelScene::Initialize()
{
	auto* pGridMap{ new GridMap{ 19, 13, 10 } };
	AddChild(pGridMap);
}
