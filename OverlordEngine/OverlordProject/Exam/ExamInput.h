#pragma once
namespace ExamInput
{
	enum class InputActions
	{
		// UI Actions
		Up,
		Down,
		Left,
		Right,
		Submit,

		// Player Actions
		MoveNorth,
		MoveEast,
		MoveSouth,
		MoveWest,
		PlaceBomb,
	};

	constexpr int PlayerActionCount{ 5 };
}
