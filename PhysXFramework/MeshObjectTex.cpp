#include "stdafx.h"
#include "MeshObjectTex.h"
#include "VertexStructs.h"
#include "ContentManager.h"
#include "MeshData.h"
#include "DdsTexResource.h"

MeshObjectTex::MeshObjectTex(const wstring& assetFile, const wstring& textureFile)
	: m_AssetFile{ assetFile }
	, m_TextureFile{ textureFile }
{
}

MeshObjectTex::~MeshObjectTex()
{
	SafeRelease(m_pVertexLayout);
}

void MeshObjectTex::Initialize()
{
	//Effect
	m_pEffect = ContentManager::GetInstance()->Load<ID3DX11Effect>(L"Resources/Effects/PosNormTex3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	m_pDiffuseVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	//InputLayout
	constexpr D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	constexpr UINT numElements = static_cast<UINT>(std::size(vertexDesc));

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc);
	auto hr = Locator::GetD3D11()->pDevice->CreateInputLayout(
		vertexDesc,
		numElements,
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize,
		&m_pVertexLayout);
	Logger::GetInstance()->LogHResult(hr, L"Failed to Create InputLayout");

	//Texture
	m_pTextureData = ContentManager::GetInstance()->Load<DdsTexResource>(m_TextureFile);

	//Load MeshData
	m_pMeshData = ContentManager::GetInstance()->Load<MeshData>(m_AssetFile);
	m_pVertexBuffer = m_pMeshData->GetVertexBuffer<VertexPosNormTex>();
	m_pIndexBuffer = m_pMeshData->GetIndexBuffer();
}

void MeshObjectTex::Update(const SceneContext& sceneContext)
{
}

void MeshObjectTex::Draw(const SceneContext& sceneContext)
{
	DirectX::XMMATRIX world = XMLoadFloat4x4(&m_WorldMatrix);
	const DirectX::XMMATRIX viewProj = XMLoadFloat4x4(&sceneContext.GetCamera()->GetViewProjection());
	DirectX::XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));
	m_pDiffuseVariable->SetResource(m_pTextureData->GetShaderResourceView());

	const auto deviceContext = Locator::GetD3D11()->pDeviceContext;

	// Set vertex buffer
	constexpr UINT stride = sizeof(VertexPosNormTex);
	constexpr UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	deviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the input layout
	deviceContext->IASetInputLayout(m_pVertexLayout);

	// Set primitive topology
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render a cube
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, deviceContext);
		deviceContext->DrawIndexed(m_pMeshData->GetIndexCount(), 0, 0);
	}
}
