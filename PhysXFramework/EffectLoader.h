#pragma once
#include "ContentLoader.h"
#include <unordered_map>

class EffectLoader : public ContentLoader<ID3DX11Effect>
{
public:
	EffectLoader();
	~EffectLoader();

	// -------------------------
	// Disabling default copy constructor and default
	// assignment operator.
	// -------------------------
	EffectLoader(const EffectLoader& obj) = delete;
	EffectLoader& operator=(const EffectLoader& obj) = delete;

protected:
	virtual ID3DX11Effect* LoadContent(const std::wstring& assetFile)override;
	virtual void Destroy(ID3DX11Effect* pObjToDestroy) override;
};
