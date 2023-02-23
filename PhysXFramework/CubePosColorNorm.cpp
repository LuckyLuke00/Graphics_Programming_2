#include "stdafx.h"
#include "CubePosColorNorm.h"
#include "VertexStructs.h"
#include "ContentManager.h"

CubePosColorNorm::CubePosColorNorm(float width, float height, float depth)
	: CubePosColorNorm{ width, height, depth, DirectX::XMFLOAT4{1, 1, 1, 1 } }

{
	// overrule other ctor setting
	m_UseCustomColor = false;
}

CubePosColorNorm::CubePosColorNorm(float width, float height, float depth, DirectX::XMFLOAT4 color)
	: m_Width{ width }
	, m_Height{ height }
	, m_Depth{ depth }
	, m_CustomColor{ color }
	, m_UseCustomColor{ true }
{
}

CubePosColorNorm::~CubePosColorNorm()
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pVertexLayout);
}

void CubePosColorNorm::Initialize()
{
	//Effect
	m_pEffect = ContentManager::GetInstance()->Load<ID3DX11Effect>(L"Resources/Effects/PosColNorm3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();

	//InputLayout
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = sizeof(vertexDesc) / sizeof(vertexDesc[0]);

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

	//Vertex Buffer
	std::vector<VertexPosColNorm> vertices(24); // room for 24 vertices
	int index = 0;

	//front red
	DirectX::XMFLOAT4 color = m_UseCustomColor ? m_CustomColor : DirectX::XMFLOAT4(1, 0, 0, 1);
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, -m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 0, -1);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 0, -1);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, -m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 0, -1);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 0, -1);
	vertices[index++].Color = color;
	//back red
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, -m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 0, 1);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 0, 1);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, -m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 0, 1);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 0, 1);
	vertices[index++].Color = color;

	color = m_UseCustomColor ? m_CustomColor : DirectX::XMFLOAT4(0, 1, 0, 1);
	//left green
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, -m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(-1, 0, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(-1, 0, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, -m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(-1, 0, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(-1, 0, 0);
	vertices[index++].Color = color;

	//right green
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, -m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(1, 0, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(1, 0, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, -m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(1, 0, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(1, 0, 0);
	vertices[index++].Color = color;

	color = m_UseCustomColor ? m_CustomColor : DirectX::XMFLOAT4(0, 0, 1, 1);
	//top blue
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 1, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 1, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 1, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, 1, 0);
	vertices[index++].Color = color;

	//bottom blue
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, -m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, -1, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(-m_Width / 2, -m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, -1, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, -m_Height / 2, m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, -1, 0);
	vertices[index++].Color = color;
	vertices[index].Position = DirectX::XMFLOAT3(m_Width / 2, -m_Height / 2, -m_Depth / 2);
	vertices[index].Normal = DirectX::XMFLOAT3(0, -1, 0);
	vertices[index++].Color = color;

	D3D11_BUFFER_DESC bd{};
	D3D11_SUBRESOURCE_DATA initData{};

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosColNorm) * static_cast<UINT>(vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = vertices.data();

	hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);

	Logger::GetInstance()->LogHResult(hr, L"Failed to Create VertexBuffer");

	//Indexbuffer
	std::vector<DWORD> indices{
		0,1,2, 2,1,3,
		4,6,5, 5,6,7,
		8,10,9, 9,10,11,
		12,13,14, 14,13,15,
		16,18,17, 17,18,19,
		20,21,22, 22,21,23
	};

	m_NumIndices = static_cast<UINT>(indices.size()); // needed in Draw()

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();
	hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::GetInstance()->LogHResult(hr, L"Failed to Create IndexBuffer");
}

void CubePosColorNorm::Update(const SceneContext& sceneContext)
{
}

void CubePosColorNorm::Draw(const SceneContext& sceneContext)
{
	DirectX::XMMATRIX world = XMLoadFloat4x4(&m_WorldMatrix);
	const DirectX::XMMATRIX viewProj = XMLoadFloat4x4(&sceneContext.GetCamera()->GetViewProjection());
	DirectX::XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));

	// Set vertex buffer
	UINT stride = sizeof(VertexPosColNorm);
	UINT offset = 0;
	auto deviceContext = Locator::GetD3D11()->pDeviceContext;
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
		deviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}
