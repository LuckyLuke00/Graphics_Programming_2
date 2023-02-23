#pragma once
struct D3D11;
#include <map>

enum class GamepadIndex : DWORD
{
	playerOne = 0,
	playerTwo = 1,
	playerThree = 2,
	playerFour = 3
};

enum class InputTriggerState
{
	pressed,
	released,
	down
};

struct InputAction
{
	InputAction() :
		actionID(-1),
		triggerState(InputTriggerState::pressed),
		keyboardCode(-1),
		mouseButtonCode(-1),
		gamepadButtonCode(0),
		playerIndex(GamepadIndex::playerOne),
		isTriggered(false) {}

	InputAction(int actionID, InputTriggerState triggerState = InputTriggerState::pressed, int keyboardCode = -1, int mouseButtonCode = -1, WORD gamepadButtonCode = 0, GamepadIndex playerIndex = GamepadIndex::playerOne) :
		actionID(actionID),
		triggerState(triggerState),
		keyboardCode(keyboardCode),
		mouseButtonCode(mouseButtonCode),
		gamepadButtonCode(gamepadButtonCode),
		playerIndex(playerIndex),
		isTriggered(false) {}

	int actionID;
	InputTriggerState triggerState;
	int keyboardCode; //VK_... (Range 0x07 <> 0xFE)
	int mouseButtonCode; //VK_... (Range 0x00 <> 0x06)
	WORD gamepadButtonCode; //XINPUT_GAMEPAD_...
	GamepadIndex playerIndex;
	bool isTriggered;
};

class InputManager
{
public:
	InputManager();
	~InputManager();
	InputManager(const InputManager& t) = delete;
	InputManager& operator=(const InputManager& t) = delete;

	void Initialize();
	void Update();
	bool AddInputAction(InputAction action);
	bool IsActionTriggered(int actionID);
	static void SetEnabled(bool enabled) { m_Enabled = enabled; m_DisabledToEnabled = true; }

	POINT GetMousePosition(bool previousFrame = false) const { return (previousFrame) ? m_OldMousePosition : m_CurrMousePosition; }
	POINT GetMouseMovement() const { return m_MouseMovement; }
	DirectX::XMFLOAT2 GetThumbstickPosition(bool leftThumbstick = true, GamepadIndex playerIndex = GamepadIndex::playerOne);
	float GetTriggerPressure(bool leftTrigger = true, GamepadIndex playerIndex = GamepadIndex::playerOne);
	void SetVibration(float leftVibration, float rightVibration, GamepadIndex playerIndex = GamepadIndex::playerOne);

	void CursorVisible(bool visible) { ShowCursor(visible); }
	bool IsGamepadConnected(GamepadIndex index) const { return m_ConnectedGamepads[(DWORD)index]; }

	bool IsKeyboardKey(InputTriggerState state, int key);
	bool IsMouseButton(InputTriggerState state, int button);
	bool IsGamepadButton(InputTriggerState state, WORD button, GamepadIndex playerIndex = GamepadIndex::playerOne);

	//bool IsKeyboardKeyDown(int key, bool previousFrame = false);
	//bool IsMouseButtonDown(int button, bool previousFrame = false);
	//bool IsGamepadButtonDown(WORD button, GamepadIndex playerIndex = GamepadIndex::playerOne, bool previousFrame = false);

private:

	std::map<int, InputAction> m_InputActions;
	BYTE* m_pCurrKeyboardState, * m_pOldKeyboardState, * m_pKeyboardState0, * m_pKeyboardState1;
	bool m_KeyboardState0Active;
	POINT m_CurrMousePosition, m_OldMousePosition, m_MouseMovement;

	XINPUT_STATE m_OldGamepadState[XUSER_MAX_COUNT], m_CurrGamepadState[XUSER_MAX_COUNT];
	bool m_ConnectedGamepads[XUSER_MAX_COUNT];
	static bool m_Enabled, m_DisabledToEnabled;

	void UpdateGamepadStates();
	bool UpdateKeyboardStates();

	bool IsKeyboardKeyDown_unsafe(int key, bool previousFrame = false);
	bool IsMouseButtonDown_unsafe(int button, bool previousFrame = false);
	bool IsGamepadButtonDown_unsafe(WORD button, GamepadIndex playerIndex = GamepadIndex::playerOne, bool previousFrame = false);

	const int KBCODE_MIN{ 7 };
	const int KBCODE_MAX{ 254 };
	const int MSCODE_MIN{ 0 };
	const int MSCODE_MAX{ 6 };
	const WORD GPCODE_MIN{ 0 };
	const WORD GPCODE_MAX{ 0x8000 };

	bool IsKeyCodeValid(int code) const { return code > KBCODE_MIN && code < KBCODE_MAX; }
	bool IsMouseCodeValid(int code) const { return code > MSCODE_MIN && code < MSCODE_MAX; }
	bool IsGamepadCodeValid(DWORD code) const { return code > GPCODE_MIN && code < GPCODE_MAX; }
};
