#include "stdafx.h"
#include "Explosion.h"
#include "Materials/DiffuseMaterial.h"
#include "Prefabs/Exam/Level/GridMap.h"
#include "Prefabs/Exam/Player/Player.h"

Explosion::Explosion(const std::wstring& model, const std::wstring& texture, float lifeTime) :
	m_LifeTime{ lifeTime }
{
	DiffuseMaterial* pMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial>() };
	if (!texture.empty()) pMaterial->SetDiffuseTexture(texture);

	AddComponent(new ModelComponent{ model })->SetMaterial(pMaterial);
}

void Explosion::Initialize(const SceneContext&)
{
	SetScale(.01f, .01f);
}

void Explosion::Update(const SceneContext& sceneContext)
{
	Player* pPlayer{ dynamic_cast<Player*>(GetGridMap()->GetGridObjectAt(GetPosition())) };
	if (pPlayer) pPlayer->Kill();

	m_Timer += sceneContext.pGameTime->GetElapsed();
	if (m_Timer >= m_LifeTime)
	{
		m_Timer = .0f;
		MarkForDelete();
	}
}
