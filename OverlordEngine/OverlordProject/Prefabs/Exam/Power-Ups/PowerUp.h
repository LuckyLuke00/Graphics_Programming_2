#pragma once
#include "Prefabs/Exam/Level/GridObject.h"

class PowerUp : public GridObject
{
public:
	explicit PowerUp() = default;
	~PowerUp() override = default;

	PowerUp(const PowerUp& other) = delete;
	PowerUp(PowerUp&& other) noexcept = delete;
	PowerUp& operator=(const PowerUp& other) = delete;
	PowerUp& operator=(PowerUp&& other) noexcept = delete;

	void PickUp();
	virtual void Use() = 0;
};
