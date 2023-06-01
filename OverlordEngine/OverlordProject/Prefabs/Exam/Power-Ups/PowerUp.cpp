#include "stdafx.h"
#include "PowerUp.h"
#include "Materials/DiffuseMaterial.h"

void PowerUp::PickUp()
{
	Use();
	MarkForDelete();
}
