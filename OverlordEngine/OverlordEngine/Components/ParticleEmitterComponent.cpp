#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount) :
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	TODO_W9(L"Implement Destructor")
}

void ParticleEmitterComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	TODO_W9(L"Implement Initialize")
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& /*sceneContext*/)
{
	TODO_W9(L"Implement CreateVertexBuffer")
}

void ParticleEmitterComponent::Update(const SceneContext& /*sceneContext*/)
{
	TODO_W9(L"Implement Update")
}

void ParticleEmitterComponent::UpdateParticle(Particle& /*p*/, float /*elapsedTime*/) const
{
	TODO_W9(L"Implement UpdateParticle")
}

void ParticleEmitterComponent::SpawnParticle(Particle& /*p*/)
{
	TODO_W9(L"Implement SpawnParticle")
}

void ParticleEmitterComponent::PostDraw(const SceneContext& /*sceneContext*/)
{
	TODO_W9(L"Implement PostDraw")
}

void ParticleEmitterComponent::DrawImGui()
{
	if (ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}
