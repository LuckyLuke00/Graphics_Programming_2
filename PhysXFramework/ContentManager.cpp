#include "stdafx.h"
#include "ContentManager.h"
#include "EffectLoader.h"
#include "DdsTexLoader.h"
#include "MeshDataLoader.h"
#include "PxTriangleMeshLoader.h"
#include "PxConvexMeshLoader.h"

ContentManager::ContentManager()
	: m_IsInitialized{ false }
	, m_pDevice{}
{
}

ContentManager::~ContentManager()
{
	for (BaseLoader* ldr : m_Loaders)
	{
		ldr->Unload();
		SafeDelete(ldr);
	}

	m_Loaders.clear();
}

void ContentManager::Initialize(ID3D11Device* pDevice)
{
	if (!m_IsInitialized)
	{
		m_pDevice = pDevice;
		m_IsInitialized = true;
		AddLoader(new EffectLoader());
		AddLoader(new DdsTexLoader());
		AddLoader(new MeshDataLoader());
		AddLoader(new PxTriangleMeshLoader());
		AddLoader(new PxConvexMeshLoader());
	}
}

void ContentManager::AddLoader(BaseLoader* loader)
{
	for (BaseLoader* ldr : m_Loaders)
	{
		if (ldr->GetType() == loader->GetType())
		{
			SafeDelete(loader);
			return;
		}
	}

	m_Loaders.push_back(loader);
	loader->SetDevice(m_pDevice);
}
