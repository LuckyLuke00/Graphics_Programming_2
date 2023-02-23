#pragma once
#include "GameObject.h"

class SpherePosColorNorm final : public GameObject
{
public:
	SpherePosColorNorm(float radius, int slices, int stacks, DirectX::XMFLOAT4 color);
	~SpherePosColorNorm() override;

	SpherePosColorNorm(const SpherePosColorNorm& t) = delete;
	SpherePosColorNorm& operator=(const SpherePosColorNorm& t) = delete;
	SpherePosColorNorm(const SpherePosColorNorm&& t) = delete;
	SpherePosColorNorm& operator=(const SpherePosColorNorm&& t) = delete;

	virtual void Initialize() override;
	virtual void Update(const SceneContext& sceneContext) override;
	virtual void Draw(const SceneContext& sceneContext) override;
	DirectX::XMFLOAT4 GetColor() const { return m_CustomColor; }

private:
	DirectX::XMFLOAT4 m_CustomColor{};
	bool m_UseCustomColor{};
	int m_NumIndices{};
	float m_Radius{};
	int m_Slices{}, m_Stacks{};

	ID3D11InputLayout* m_pVertexLayout{};
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	ID3DX11Effect* m_pEffect{};
	ID3DX11EffectTechnique* m_pTechnique{};
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable{};
};
