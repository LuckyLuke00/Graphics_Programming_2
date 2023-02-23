#include "stdafx.h"
#include "PxConvexMeshLoader.h"
#include "PhysxManager.h"

#pragma warning( disable : 4244 ) // ignore the warning caused by string(assetFile.begin(), assetFile.end());

PxConvexMeshLoader::PxConvexMeshLoader()
{
}

PxConvexMeshLoader::~PxConvexMeshLoader()
{
}

PxConvexMesh* PxConvexMeshLoader::LoadContent(const std::wstring& assetFile)
{
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	string buffer = string(assetFile.begin(), assetFile.end());
	auto inputStream = PxDefaultFileInputData(buffer.c_str());
	auto convexMesh = physX->createConvexMesh(inputStream);

	return convexMesh;
}

void PxConvexMeshLoader::Destroy(PxConvexMesh* pObjToDestroy)
{
	//SafeDelete(pObjToDestroy);
}
