#pragma once
#include "ContentLoader.h"
#include "DdsTexResource.h"

class PxConvexMeshLoader : public ContentLoader<PxConvexMesh>
{
public:
	PxConvexMeshLoader();
	~PxConvexMeshLoader();

	// -------------------------
	// Disabling default copy constructor and default
	// assignment operator.
	// -------------------------
	PxConvexMeshLoader(const PxConvexMeshLoader& yRef) = delete;
	PxConvexMeshLoader& operator=(const PxConvexMeshLoader& yRef) = delete;

protected:

	virtual PxConvexMesh* LoadContent(const std::wstring& assetFile)override;
	virtual void Destroy(PxConvexMesh* pObjToDestroy)override;
};
