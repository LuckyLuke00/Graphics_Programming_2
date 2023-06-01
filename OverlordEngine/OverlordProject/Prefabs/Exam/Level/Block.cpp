#include "stdafx.h"
#include "Block.h"
#include "Materials/DiffuseMaterial.h"
#include "Components/PowerUpSpawnerComponent.h"

Block::Block(const std::wstring& model, const std::wstring& texture, bool isBreakable) :
	m_IsBreakable{ isBreakable },
	m_TexturePath{ texture }
{
	DiffuseMaterial* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	if (!m_TexturePath.empty()) pMaterial->SetDiffuseTexture(m_TexturePath);

	auto* pModel{ AddComponent(new ModelComponent{ model }) };
	pModel->SetMaterial(pMaterial);

	if (m_IsBreakable)
	{
		m_pSpawner = AddComponent(new PowerUpSpawnerComponent{});
	}
}

void Block::Break()
{
	m_pSpawner->SpawnPowerUp(GetPosition(), .25f);
	MarkForDelete();
}
