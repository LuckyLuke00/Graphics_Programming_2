#include "stdafx.h"
#include "DdsTexResource.h"

DdsTexResource::DdsTexResource(ID3D11Resource* pTexture, ID3D11ShaderResourceView* pTextureShaderResourceView) :
	m_pTexture(pTexture),
	m_pTextureShaderResourceView(pTextureShaderResourceView)
{
}

DdsTexResource::~DdsTexResource()
{
	SafeRelease(m_pTexture);
	SafeRelease(m_pTextureShaderResourceView);
}
