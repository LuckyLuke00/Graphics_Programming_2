#pragma once
#include "stdafx.h"

class DdsTexResource
{
public:
	DdsTexResource(ID3D11Resource* pTexture, ID3D11ShaderResourceView* pTextureShaderResourceView);
	~DdsTexResource();
	DdsTexResource(const DdsTexResource& t) = delete;
	DdsTexResource& operator=(const DdsTexResource& t) = delete;

	ID3D11Resource* GetResource() const { return m_pTexture; }
	ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pTextureShaderResourceView; }

private:

	ID3D11Resource* m_pTexture;
	ID3D11ShaderResourceView* m_pTextureShaderResourceView;
};
