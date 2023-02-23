#pragma once
#include "GameObject.h"

class TorusPosColorNorm final : public GameObject
{
public:
	TorusPosColorNorm(float majorRadius, UINT majorRadiusSteps, float minorRadius, UINT minorRadiusSteps, DirectX::XMFLOAT4 color = static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Green));
	~TorusPosColorNorm() override;

	TorusPosColorNorm(const TorusPosColorNorm& t) = delete;
	TorusPosColorNorm& operator=(const TorusPosColorNorm& t) = delete;
	TorusPosColorNorm(const TorusPosColorNorm&& t) = delete;
	TorusPosColorNorm& operator=(const TorusPosColorNorm&& t) = delete;

	void Initialize() override;
	void Update(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& sceneContext) override;

private:

	float m_MajorRadius{}, m_MinorRadius{};
	UINT m_MajorRadiusSteps{}, m_MinorRadiusSteps{}, m_NumVerts{}, m_NumIndices{};
	DirectX::XMFLOAT4 m_Color{};

	ID3D11InputLayout* m_pVertexLayout{};
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	ID3DX11Effect* m_pEffect{};
	ID3DX11EffectTechnique* m_pTechnique{};
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable{};
};
