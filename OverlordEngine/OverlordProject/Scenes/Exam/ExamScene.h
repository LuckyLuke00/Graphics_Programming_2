#pragma once

class ExamScene final : public GameScene
{
public:
	ExamScene() : GameScene(L"ExamScene Test Scene") {}
	~ExamScene() override = default;

	ExamScene(const ExamScene& other) = delete;
	ExamScene(ExamScene&& other) noexcept = delete;
	ExamScene& operator=(const ExamScene& other) = delete;
	ExamScene& operator=(ExamScene&& other) noexcept = delete;

protected:
	void Initialize() override;

private:

	enum class Menus
	{
		MainMenu,
		None
	};

	// Function that loads and opens the specified menu
	void LoadMenu(const Menus& menu);
	void LoadSprites();

	// Container that holds all the menu's
	std::map<Menus, GameObject*> m_pMenuBackgroundSprites{};

	// Variable that stores the current menu
	Menus m_CurrentMenu{ Menus::None };
};
