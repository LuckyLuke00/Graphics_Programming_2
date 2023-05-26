#include "stdafx.h"
#include "Block.h"
#include <Materials/DiffuseMaterial.h>

Block::Block(const std::wstring& model, const std::wstring& texture, bool isBreakable) :
	m_IsBreakable{ isBreakable },
	m_TexturePath{ texture }
{
	DiffuseMaterial* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	if (!m_TexturePath.empty()) pMaterial->SetDiffuseTexture(m_TexturePath);

	auto* pModel{ AddComponent(new ModelComponent{ model }) };
	pModel->SetMaterial(pMaterial);
}
