//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "PongScene.h"

PongScene::PongScene() :
	GameScene(L"PongScene") {}

void PongScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;
}

void PongScene::Update()
{
}

void PongScene::Draw()
{
	//Optional
}

void PongScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);
}
