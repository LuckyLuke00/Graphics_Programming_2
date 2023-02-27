#include "stdafx.h"
#include "AudioTestScene.h"
#include "SpherePosColorNorm.h"
#include "SoundManager.h"

#include "PhysxManager.h"
#include "Logger.h"

void AudioTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX{ PhysxManager::GetInstance()->GetPhysics() };
	auto pDefaultMaterial{ pPhysX->createMaterial(.5f, .5f, .2f) };

	// Ground Plane
	const auto pGroundActor{ pPhysX->createRigidStatic(physx::PxTransform{ { PxPiDivTwo, { 0, 0, 1 } } }) };
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	// Sphere
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere);

	const auto pSphereActor{ pPhysX->createRigidDynamic(PxTransform{ PxIdentity }) };
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(.0f, 2.f, .0f);

	//INPUT
	const auto pInput{ m_SceneContext.GetInput() };
	pInput->AddInputAction(InputAction{ InputIDs::Play2D, InputTriggerState::pressed, VK_SPACE });
	pInput->AddInputAction(InputAction{ InputIDs::Play3D, InputTriggerState::pressed, 'B' });
	pInput->AddInputAction(InputAction{ InputIDs::IncreaeVolume, InputTriggerState::pressed, VK_UP });
	pInput->AddInputAction(InputAction{ InputIDs::DecreaseVolume, InputTriggerState::pressed, VK_DOWN });
	const auto pFmod{ SoundManager::GetInstance()->GetSystem() };

	//SOUND 2D
	FMOD::Sound* pSound2D{ nullptr };
	FMOD_RESULT result{ pFmod->createStream("Resources/Sounds/wave.mp3", FMOD_2D | FMOD_LOOP_NORMAL, nullptr, &pSound2D) };
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound2D, nullptr, true, &m_pChannel2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	//SOUND 3D
	FMOD::Sound* pSound3D{ nullptr };
	result = pFmod->createStream("Resources/Sounds/Thrones.mp3", FMOD_3D | FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF, nullptr, &pSound3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound3D, nullptr, true, &m_pChannel3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	m_pChannel3D->set3DMinMaxDistance(.0f, 20.f);

}

inline FMOD_VECTOR ToFmod(XMFLOAT3 v)
{
	return { v.x, v.y, v.z };
}

inline FMOD_VECTOR ToFmod(PxVec3 v)
{
	return { v.x, v.y, v.z };
}

void AudioTestScene::Update()
{
	//Sphere
	constexpr float force{ 10.f };
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, force });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, -force });
	}

	// Sound Input
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIDs::Play2D))
	{
		bool bPaused{ false };
		m_pChannel2D->getPaused(&bPaused);
		m_pChannel2D->setPaused(!bPaused);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIDs::Play3D))
	{
		bool bPaused{ false };
		m_pChannel3D->getPaused(&bPaused);
		m_pChannel3D->setPaused(!bPaused);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIDs::IncreaeVolume))
	{
		float volume{ .0f };
		m_pChannel2D->getVolume(&volume);

		volume += .1f;
		Clamp(volume, 10.f, .0f);
		m_pChannel2D->setVolume(volume);

		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume Changed >> %f", volume);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(InputIDs::DecreaseVolume))
	{
		float volume{ .0f };
		m_pChannel2D->getVolume(&volume);

		volume -= .1f;
		Clamp(volume, 10.f, .0f);
		m_pChannel2D->setVolume(volume);

		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume Changed >> %f", volume);
	}

	//3D Sound Attributes
	const auto pCam{ m_SceneContext.GetCamera() };
	auto pos{ ToFmod(pCam->GetPosition()) };
	auto forward{ ToFmod(pCam->GetForward()) };
	auto up{ ToFmod(pCam->GetUp()) };

	FMOD_VECTOR vel{};
	const float dt{ m_SceneContext.GetGameTime()->GetElapsed() };
	vel.x = (pos.x - m_PrevCamPos.x) / dt;
	vel.y = (pos.y - m_PrevCamPos.y) / dt;
	vel.z = (pos.z - m_PrevCamPos.z) / dt;
	m_PrevCamPos = pos;

	SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
	auto spherePos{ ToFmod(m_pSphere->GetPosition()) };
	auto sphereVel{ ToFmod(m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->getLinearVelocity()) };
	m_pChannel3D->set3DAttributes(&spherePos, &sphereVel);
}

void AudioTestScene::Draw() const
{
}

void AudioTestScene::OnSceneActivated()
{
}

void AudioTestScene::OnSceneDeactivated()
{
}
