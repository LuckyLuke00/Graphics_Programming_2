#pragma once
#include "GameObject.h"

class CubePosColorNorm final : public GameObject
{
public:
	CubePosColorNorm(float width, float height, float depth);
	CubePosColorNorm(float width, float height, float depth, DirectX::XMFLOAT4 color);
	~CubePosColorNorm() override;

	CubePosColorNorm(const CubePosColorNorm& other) = delete;
	CubePosColorNorm& operator=(const CubePosColorNorm& other) = delete;
	CubePosColorNorm(const CubePosColorNorm&& other) = delete;
	CubePosColorNorm& operator=(const CubePosColorNorm&& other) = delete;

	void Initialize() override;
	void Update(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& sceneContext) override;

private:
	UINT m_NumIndices{};
	float m_Width{}, m_Height{}, m_Depth{};

	DirectX::XMFLOAT4 m_CustomColor{};
	bool m_UseCustomColor{};

	ID3D11InputLayout* m_pVertexLayout{};
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	ID3DX11Effect* m_pEffect{};
	ID3DX11EffectTechnique* m_pTechnique{};
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable{};
};
