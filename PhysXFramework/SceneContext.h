#pragma once
#include "GameTime.h"
#include "BaseCamera.h"
#include "InputManager.h"

class SceneContext final
{
public:
	SceneContext();
	~SceneContext();
	SceneContext(const SceneContext& t) = delete;
	SceneContext& operator=(const SceneContext& t) = delete;

	BaseCamera* GetCamera() const { return m_pCamera; }
	void SetCamera(BaseCamera* pCamera);

	GameTime* GetGameTime() const { return m_pGameTime; }
	InputManager* GetInput() const { return m_pInput; }
private:

	GameTime* m_pGameTime;
	BaseCamera* m_pCamera;
	InputManager* m_pInput;
};
