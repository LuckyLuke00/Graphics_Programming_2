#pragma once

#include "Singleton.h"

class PhysxAllocator;
class PhysxErrorCallback;
class GameScene;

#pragma warning(disable : 4100) // disable warning C4100: 'some variable': unreferenced formal parameter

class PhysxManager : public Singleton<PhysxManager>
{
public:

	~PhysxManager();
	PhysxManager(const PhysxManager& obj) = delete;
	PhysxManager& operator=(const PhysxManager& obj) = delete;

	//GETTERS
	PxPhysics* GetPhysics() const { return m_pPhysics; }
	PxPvd* GetPvd() const { return m_pPvd; }

	//Methods
	PxScene* CreateScene(GameScene* pScene);

	bool ToggleVisualDebuggerConnection();
	bool IsPvdConnected() const { return m_pPvd && m_pPvd->isConnected(); }

protected:
	PhysxManager();
	friend Singleton<PhysxManager>; // allow Singleton to call the constructor

private:

	void Init();
	void CleanUp();

	PhysxAllocator* m_pDefaultAllocator{ nullptr };
	PhysxErrorCallback* m_pDefaultErrorCallback{ nullptr };
	PxFoundation* m_pFoundation{ nullptr };
	PxPvd* m_pPvd{ nullptr };
	PxPhysics* m_pPhysics{ nullptr };
	PxDefaultCpuDispatcher* m_pDefaultCpuDispatcher{ nullptr };
};
