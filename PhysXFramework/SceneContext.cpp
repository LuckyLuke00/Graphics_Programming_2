#include "stdafx.h"
#include "SceneContext.h"
#include "GameTime.h"
#include "Logger.h"
//GameContext* GameContext::m_pInitContext{ nullptr };

SceneContext::SceneContext() :
	m_pGameTime{ new GameTime() },
	m_pCamera{ nullptr },
	m_pInput{ new InputManager() }
{
}

SceneContext::~SceneContext()
{
	SafeDelete(m_pGameTime);
	SafeDelete(m_pInput);
}

void SceneContext::SetCamera(BaseCamera* pCamera)
{
	if (!pCamera)
		Logger::GetInstance()->LogWarning(L"GameContext::SetCamera(nullptr)!");

	m_pCamera = pCamera;
}
