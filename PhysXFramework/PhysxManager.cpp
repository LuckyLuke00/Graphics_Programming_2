#include "stdafx.h"
#include "PhysxManager.h"
#include "PhysxAllocator.h"
#include "PhysxErrorCallback.h"
#include "GameScene.h"
#include "Logger.h"
#include "CustomSimulationFilterShader.h"

#define PVD_HOST "127.0.0.1"
#define PVD_PORT 5425

PhysxManager::PhysxManager()
{
	Init();
}

PhysxManager::~PhysxManager()
{
	CleanUp();
}

void PhysxManager::CleanUp()
{
	if (m_pPhysics)
	{
		PxCloseExtensions();
		m_pPhysics->release();
	}

	if (m_pDefaultCpuDispatcher)
	{
		m_pDefaultCpuDispatcher->release();
	}

	if (m_pPvd)
	{
		if (m_pPvd->getTransport())
		{
			m_pPvd->getTransport()->release();
		}

		m_pPvd->release();
	}

	//@END of release chain!
	if (m_pFoundation)
	{
		m_pFoundation->release();
	}

	SafeDelete(m_pDefaultAllocator);
	SafeDelete(m_pDefaultErrorCallback);
}

void PhysxManager::Init()
{
	//Recording memory allocations is necessary if you want to
	//use the memory facilities in PVD effectively.  Since PVD isn't necessarily connected
	//right away, we add a mechanism that records all outstanding memory allocations and
	//forwards them to PVD when it does connect.

	//This certainly has a performance and memory profile effect and thus should be used
	//only in non-production builds.
	bool recordMemoryAllocations = false;
#if defined(DEBUG) || defined(_DEBUG)
	recordMemoryAllocations = true;
#endif

	//WIN32 Allocator Implementation
	m_pDefaultAllocator = new PhysxAllocator();
	m_pDefaultErrorCallback = new PhysxErrorCallback();
	//Create Foundation
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *m_pDefaultAllocator, *m_pDefaultErrorCallback);
	if (!m_pFoundation)
	{
		Logger::GetInstance()->LogError(L"Physx: Foundation creation failed!");
		return;
	}

#ifdef _DEBUG
	m_pPvd = PxCreatePvd(*m_pFoundation);
	if (!m_pPvd)
	{
		Logger::GetInstance()->LogWarning(L"Physx: PhysX Visual Debugger (PVD) creation failed! (PVD connection will not be possible)");
	}
#endif

	////Create ProfileZoneManager
	//m_pProfileZoneManager = &PxProfileZoneManager::createProfileZoneManager(m_pFoundation);
	//if(!m_pProfileZoneManager)
	//{
	//	Logger::GetInstance()->LogError(L"Physx: Profile Zone Manager creation failed!");
	//}

	//Cude context manager?

	//Create Main Physics Object
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, PxTolerancesScale(), true, m_pPvd);
	if (!m_pPhysics)
	{
		Logger::GetInstance()->LogError(L"Physx: Physics Object creation failed!");
		return;
	}

	PxInitExtensions(*m_pPhysics, m_pPvd);

	//TODO: ADD ProfileZone

	//Create Default CpuDispatcher (only 1 thread)
	m_pDefaultCpuDispatcher = PxDefaultCpuDispatcherCreate(2);

	//Try to connect with the PVD
	ToggleVisualDebuggerConnection();
}

PxScene* PhysxManager::CreateScene(GameScene* pScene)
{
	auto sceneDesc = PxSceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.setToDefault(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.cpuDispatcher = m_pDefaultCpuDispatcher;
	sceneDesc.filterShader = GP1SimulationFilterShader;
	sceneDesc.userData = pScene;
	//Link GpuDispatcher;

	auto physxScene = m_pPhysics->createScene(sceneDesc);
	if (!physxScene)
	{
		Logger::GetInstance()->LogError(L"Physx: Scene creation failed!");
	}

	return physxScene;
}

bool PhysxManager::ToggleVisualDebuggerConnection()
{
	if (!m_pPhysics || !m_pPvd) return false;

	if (m_pPvd->isConnected())
	{
		//DISCONNECT
		m_pPvd->disconnect();
		return false;
	}

	//CONNECT
	PxPvdTransport* pTransport = m_pPvd->getTransport();
	if (!pTransport)
	{
		pTransport = PxDefaultPvdSocketTransportCreate(PVD_HOST, PVD_PORT, 10);
	}

	if (m_pPvd->connect(*pTransport, PxPvdInstrumentationFlag::eALL))
	{
		return true;
	}

	return false;
}

//void PhysxManager::onPvdConnected(PxVisualDebuggerConnection& connection)
//{
//	m_pPhysics->getVisualDebugger()->setVisualizeConstraints(true);
//	//m_pPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlags::eTRANSMIT_CONTACTS, true);
//	//m_pPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlags::eTRANSMIT_SCENEQUERIES, true);
//}
