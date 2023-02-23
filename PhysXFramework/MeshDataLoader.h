#pragma once
#include "ContentLoader.h"
#include "MeshData.h"

class MeshDataLoader : public ContentLoader<MeshData>
{
public:
	MeshDataLoader() = default;
	~MeshDataLoader() = default;

	MeshDataLoader(const MeshDataLoader& obj) = delete;
	MeshDataLoader& operator=(const MeshDataLoader& obj) = delete;

protected:
	virtual MeshData* LoadContent(const std::wstring& assetFile) override;
	virtual void Destroy(MeshData* pObjToDestroy) override;

private:

	enum class MeshDataType :int
	{
		END = 0,
		HEADER = 1,
		POSITIONS = 2,
		INDICES = 3,
		NORMALS = 4,
		COLORS = 7,
		TEXCOORDS = 8
	};
};
