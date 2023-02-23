#include "stdafx.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "D3D11.h"
#include "DebugRenderer.h"

SceneManager::SceneManager()
	: m_pActiveScene{}
	, m_pNewActiveScene{}
	, m_InputEnabled{ true }
	, m_IsInitialized{ false }
	, m_pDebugRenderer{ std::make_shared<DebugRenderer>() }
{
}

SceneManager::~SceneManager()
{
	Destroy();
}

void SceneManager::Destroy()
{
	for (GameScene* pScene : m_Scenes)
	{
		SafeDelete(pScene);
	}
	m_Scenes.clear();
}

void SceneManager::AddGameScene(GameScene* pScene)
{
	auto it = find(m_Scenes.begin(), m_Scenes.end(), pScene);

	if (it == m_Scenes.end())
	{
		m_Scenes.push_back(pScene);

		if (m_IsInitialized)
			pScene->RootInitialize();

		if (m_pActiveScene == nullptr && m_pNewActiveScene == nullptr)
			m_pNewActiveScene = pScene;

		pScene->SetDebugRenderer(m_pDebugRenderer);
	}
}

void SceneManager::RemoveGameScene(GameScene* scene)
{
	auto it = find(m_Scenes.begin(), m_Scenes.end(), scene);

	if (it != m_Scenes.end())
	{
		// Delete scene before removing its pointer from the container
		SafeDelete(*it);
		m_Scenes.erase(it);
	}
}

void SceneManager::NextScene()
{
	for (size_t i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes[i] == m_pActiveScene)
		{
			auto nextScene = (++i) % m_Scenes.size();
			m_pNewActiveScene = m_Scenes[nextScene];
			return;
		}
	}
}

void SceneManager::PreviousScene()
{
	for (size_t i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes[i] == m_pActiveScene)
		{
			if (i == 0) i = m_Scenes.size();
			--i;
			m_pNewActiveScene = m_Scenes[i];
			return;
		}
	}
}

void SceneManager::SetActiveGameScene(const std::wstring& sceneName)
{
	auto it = find_if(m_Scenes.begin(), m_Scenes.end(), [sceneName](GameScene* pScene)
		{
			return (pScene->GetName() == sceneName);
		});

	if (it != m_Scenes.end())
	{
		m_pNewActiveScene = *it;
	}
}

void SceneManager::Initialize()
{
	m_pDebugRenderer->InitRenderer(Locator::GetD3D11()->pDevice);

	for (GameScene* pScene : m_Scenes)
	{
		pScene->RootInitialize();
	}
	m_IsInitialized = true;
}

void SceneManager::WindowStateChanged(bool active)
{
	if (m_pActiveScene)
	{
		m_pActiveScene->RootWindowStateChanged(active);
	}
}

void SceneManager::ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYUP:
	{
		// process scene manager related messages
		if (wParam == 'A')
		{
			// do stuff
			break;
		}
	}
	}
	if (m_pActiveScene) m_pActiveScene->ProcessMessage(message, wParam, lParam);
}

void SceneManager::ToggleDebugRenderer()
{
	m_pDebugRenderer->ToggleDebugRenderer();
}

void SceneManager::Update()
{
	if (m_pNewActiveScene != nullptr)
	{
		//Deactivate the current active scene
		if (m_pActiveScene != nullptr)
			m_pActiveScene->RootSceneDeactivated();

		//Set New Scene
		m_pActiveScene = m_pNewActiveScene;
		m_pNewActiveScene = nullptr;

		//Active the new scene and reset SceneTimer
		m_pActiveScene->RootSceneActivated();
	}

	if (m_pActiveScene != nullptr)
		m_pActiveScene->RootUpdate();
}

void SceneManager::Draw()
{
	if (m_pActiveScene) m_pActiveScene->RootDraw();
}
