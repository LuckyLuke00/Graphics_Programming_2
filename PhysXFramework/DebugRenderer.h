#pragma once
#include "Singleton.h"

class SceneContext;
struct VertexPosCol;
struct D3D11;

class DebugRenderer final
{
public:
	DebugRenderer();
	~DebugRenderer();
	DebugRenderer(const DebugRenderer& other) = delete;
	DebugRenderer(DebugRenderer&& other) noexcept = delete;
	DebugRenderer& operator=(const DebugRenderer& other) = delete;
	DebugRenderer& operator=(DebugRenderer&& other) noexcept = delete;

	void InitRenderer(ID3D11Device* pDevice, UINT bufferSize = 100);
	void ToggleDebugRenderer();
	void ToggleGrid();
	void Release();

	//RENDERING
	void DrawGrid(const SceneContext& sceneContext);
	void DrawPhysX(physx::PxScene* pScene);

private:

	void CreateVertexBuffer(ID3D11Device* pDevice);
	void CreateFixedLineList();
	void DrawLine(DirectX::XMFLOAT3 start, DirectX::XMFLOAT3 end, DirectX::XMFLOAT4 color = static_cast<DirectX::XMFLOAT4>(DirectX::Colors::Red));
	void DrawLine(DirectX::XMFLOAT3 start, DirectX::XMFLOAT4 colorStart, DirectX::XMFLOAT3 end, DirectX::XMFLOAT4 colorEnd);
	DirectX::XMFLOAT4 ConvertPxColor(physx::PxU32 color);

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;
	UINT m_BufferSize, m_FixedBufferSize;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pVertexBuffer;
	ID3DX11EffectMatrixVariable* m_pWvpVariable;
	physx::PxScene* m_pPhysxDebugScene;

	bool m_RendererEnabled{ true };
	bool m_GridEnabled{ true };

	std::vector<VertexPosCol> m_LineList;
	std::vector<VertexPosCol> m_FixedLineList;
};
