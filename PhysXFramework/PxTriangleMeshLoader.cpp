#include "stdafx.h"
#include "PxTriangleMeshLoader.h"
#include "PhysxManager.h"

#pragma warning( disable : 4244 ) // disable warnings caused by string buffer{ assetFile.begin(), assetFile.end() };

PxTriangleMeshLoader::PxTriangleMeshLoader()
{
}

PxTriangleMeshLoader::~PxTriangleMeshLoader()
{
}

PxTriangleMesh* PxTriangleMeshLoader::LoadContent(const std::wstring& assetFile)
{
	PxPhysics* pPhysX = PhysxManager::GetInstance()->GetPhysics();

	string buffer{ assetFile.begin(), assetFile.end() };
	auto inputStream = PxDefaultFileInputData(buffer.c_str());
	auto triangleMesh = pPhysX->createTriangleMesh(inputStream);

	return triangleMesh;
}

void PxTriangleMeshLoader::Destroy(PxTriangleMesh* pObjToDestroy)
{
	//SafeDelete(pObjToDestroy);
}
