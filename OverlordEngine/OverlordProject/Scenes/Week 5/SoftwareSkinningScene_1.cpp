#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Prefabs/BoneObject.h"
#include "Materials/ColorMaterial.h"

void SoftwareSkinningScene_1::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;

	// Create a ColorMaterial that will be used for both BoneObjects
	const auto pMaterial{ MaterialManager::Get()->CreateMaterial<ColorMaterial>() };

	// Create a new GameObject which will act as the root for our Bone Hierarchy (pRoot)
	const auto pRoot{ new GameObject() };

	// Initialize the first bone (m_pBone0), use the premade ColorMaterial and a length of
	// 15.f.This bone is then added to the pRoot as a child.
	m_pBone0 = new BoneObject(pMaterial, 15.f);
	pRoot->AddChild(m_pBone0);

	// Initialize the second bone (m_pBone1), same parameters as m_pBone0.
	// This bone is added as a child to m_pBone0 BUT use the AddBone function instead of the
	// AddChild function!(The AddBone function will add this bone as a child and
	// translate it to the end of the parent bone)
	m_pBone1 = new BoneObject(pMaterial, 15.f);
	m_pBone0->AddBone(m_pBone1);

	// Make sure to add pRoot to the scenegraph using AddChild
	AddChild(pRoot);
}

void SoftwareSkinningScene_1::Update()
{
	if (m_AutoRotate)
	{
		constexpr float rotationSpeed{ 45.f };
		m_BoneRotation += rotationSpeed * static_cast<float>(m_RotationSign) * GetSceneContext().pGameTime->GetElapsed();
		m_RotationSign *= (m_BoneRotation > 45.f || m_BoneRotation < -45.f) ? -1 : 1;

		m_RotBone0.z = m_BoneRotation;
		m_RotBone1.z = -m_BoneRotation * 2.f;
	}

	m_pBone0->GetTransform()->Rotate(m_RotBone0);
	m_pBone1->GetTransform()->Rotate(m_RotBone1);
}

void SoftwareSkinningScene_1::OnGUI()
{
	ImGui::BeginDisabled(m_AutoRotate);
	ImGui::DragFloat3("Bone 0 - ROT", &m_RotBone0.x);
	ImGui::DragFloat3("Bone 1 - ROT", &m_RotBone1.x);
	ImGui::EndDisabled();

	ImGui::Checkbox("Auto Rotate", &m_AutoRotate);
}
