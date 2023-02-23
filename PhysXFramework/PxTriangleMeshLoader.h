#pragma once
#include "ContentLoader.h"
#include "DdsTexResource.h"

class PxTriangleMeshLoader : public ContentLoader<PxTriangleMesh>
{
public:
	PxTriangleMeshLoader();
	~PxTriangleMeshLoader();
	// -------------------------
	// Disabling default copy constructor and default
	// assignment operator.
	// -------------------------
	PxTriangleMeshLoader(const PxTriangleMeshLoader& yRef) = delete;
	PxTriangleMeshLoader& operator=(const PxTriangleMeshLoader& yRef) = delete;
protected:

	virtual PxTriangleMesh* LoadContent(const std::wstring& assetFile)override;
	virtual void Destroy(PxTriangleMesh* pObjToDestroy)override;
};
