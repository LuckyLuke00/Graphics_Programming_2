#include "stdafx.h"
#include "FirePowerUp.h"
#include "Materials/DiffuseMaterial.h"

FirePowerUp::FirePowerUp(const std::wstring& model, const std::wstring& texture)
{
	DiffuseMaterial* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	if (!texture.empty()) pMaterial->SetDiffuseTexture(texture);

	auto* pModel{ AddComponent(new ModelComponent{ model }) };
	pModel->SetMaterial(pMaterial);
}

void FirePowerUp::Use()
{
}
