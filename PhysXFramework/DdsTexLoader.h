#pragma once
#include "ContentLoader.h"
#include "DdsTexResource.h"

class DdsTexLoader : public ContentLoader<DdsTexResource>
{
public:
	DdsTexLoader();
	~DdsTexLoader();
	DdsTexLoader(const DdsTexLoader& yRef) = delete;
	DdsTexLoader& operator=(const DdsTexLoader& yRef) = delete;
protected:

	virtual DdsTexResource* LoadContent(const std::wstring& assetFile) override;
	virtual void Destroy(DdsTexResource* pObjToDestroy) override;
};
