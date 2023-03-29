#include "stdafx.h"
#include "SceneSelector.h"
#include "SceneManager.h"

//Change this define to activate/deactivate the corresponding scenes
// W01 - W02  (#define ...)

/*LAB Content*/
//#define W01
//#define W02

/*MILESTONE Content*/
#define MILESTONE_1

#ifdef W01
#include "TestScene.h"
#include "PhysXTestScene.h"
#include "BouncingBallScene.h"
#include "FrictionTestScene.h"
#include "BoxForceScene.h"
#include "BoxTorqueScene.h"
#include "W1_AssignmentScene.h"
#endif

#ifdef W02
#include "PhysXMeshTestScene.h"
#include "TriggerTestScene.h"
#include "AudioTestScene.h"
#include "W2_AssignmentScene.h"
#endif

#ifdef MILESTONE_1
#include "W1_AssignmentScene.h"
#include "W2_AssignmentScene.h"
#endif

namespace dae
{
	void AddScenes(SceneManager* pSceneManager)
	{
#ifdef W01
		pSceneManager->AddGameScene(new TestScene());
		pSceneManager->AddGameScene(new PhysXTestScene());
		pSceneManager->AddGameScene(new BouncingBallScene());
		pSceneManager->AddGameScene(new FrictionTestScene());
		pSceneManager->AddGameScene(new BoxForceScene());
		pSceneManager->AddGameScene(new BoxTorqueScene());
		pSceneManager->AddGameScene(new W1_AssignmentScene());
#endif

#ifdef W02
		pSceneManager->AddGameScene(new PhysXMeshTestScene());
		pSceneManager->AddGameScene(new TriggerTestScene());
		pSceneManager->AddGameScene(new AudioTestScene());
		pSceneManager->AddGameScene(new W2_AssignmentScene());
#endif

#ifdef MILESTONE_1
		pSceneManager->AddGameScene(new W1_AssignmentScene());
		pSceneManager->AddGameScene(new W2_AssignmentScene());
#endif
	}
}
