#pragma once
#include "GameObject.h"

class CylinderPosColorNorm final : public GameObject
{
public:
	CylinderPosColorNorm(float radius, UINT radiusSteps, float height, DirectX::XMFLOAT4 color = static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Green));
	~CylinderPosColorNorm() override;

	CylinderPosColorNorm(const CylinderPosColorNorm& other) = delete;
	CylinderPosColorNorm& operator=(const CylinderPosColorNorm& other) = delete;
	CylinderPosColorNorm(const CylinderPosColorNorm&& other) = delete;
	CylinderPosColorNorm& operator=(const CylinderPosColorNorm&& other) = delete;

	void Initialize() override;
	void Update(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& sceneContext) override;

private:

	float m_Radius{}, m_Height{};
	UINT m_RadiusSteps{}, m_NumIndices{};
	DirectX::XMFLOAT4 m_Color{};

	ID3D11InputLayout* m_pVertexLayout{};
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	ID3DX11Effect* m_pEffect{};
	ID3DX11EffectTechnique* m_pTechnique{};
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable{};
};
