#include "stdafx.h"
#include "PowerUp.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/Exam/Player/Player.h"
#include "Prefabs/Exam/Level/GridMap.h"

void PowerUp::PickUp(Player* pPlayer)
{
	if (!pPlayer) return;

	Use(pPlayer);

	MarkForDelete();
}
