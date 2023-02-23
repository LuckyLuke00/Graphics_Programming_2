#include "stdafx.h"
#include "CylinderPosColorNorm.h"
#include "VertexStructs.h"
#include "ContentManager.h"
#include "Logger.h"
#include <vector>

CylinderPosColorNorm::CylinderPosColorNorm(float radius, UINT radiusSteps, float height, DirectX::XMFLOAT4 color)
	: m_Radius{ radius }
	, m_Height{ height }
	, m_RadiusSteps{ radiusSteps }
	, m_Color{ color }
{
}

CylinderPosColorNorm::~CylinderPosColorNorm()
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pVertexLayout);
}

void CylinderPosColorNorm::Initialize()
{
	//Effect
	m_pEffect = ContentManager::GetInstance()->Load<ID3DX11Effect>(L"Resources/Effects/PosColNorm3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pMatWorldViewProjVariable->IsValid())Logger::GetInstance()->LogWarning(L"CylinderPosColorNorm::Initialize() - WvpVariable is invalid!");
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	if (!m_pMatWorldVariable->IsValid())Logger::GetInstance()->LogWarning(L"CylinderPosColorNorm::Initialize() - WorldVariable is invalid!");

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

	//**CYLINDER**

	//Vertices
	vector<VertexPosColNorm> vertices;
	float interval = DirectX::XM_2PI / m_RadiusSteps;
	for (UINT step = 0; step < m_RadiusSteps; ++step)
	{
		float circleRadius = step * interval;
		XMFLOAT3 pos = DirectX::XMFLOAT3(m_Radius * cos(circleRadius), m_Height / 2.0f, m_Radius * sin(circleRadius));
		XMVECTOR vecNorm = XMVectorSet(pos.x, 0, pos.z, 0);
		vecNorm = XMVector3Normalize(vecNorm);
		DirectX::XMFLOAT3 norm;
		XMStoreFloat3(&norm, vecNorm);

		vertices.push_back(VertexPosColNorm(pos, m_Color, DirectX::XMFLOAT3(0, 1, 0)));

		vertices.push_back(VertexPosColNorm(pos, m_Color, norm));
		pos.y *= -1;
		vertices.push_back(VertexPosColNorm(pos, m_Color, norm));
		vertices.push_back(VertexPosColNorm(pos, m_Color, DirectX::XMFLOAT3(0, -1, 0)));
	}

	vertices.push_back(VertexPosColNorm(DirectX::XMFLOAT3(0, m_Height / 2.0f, 0), m_Color, DirectX::XMFLOAT3(0, 1, 0)));
	vertices.push_back(VertexPosColNorm(DirectX::XMFLOAT3(0, -m_Height / 2.0f, 0), m_Color, DirectX::XMFLOAT3(0, -1, 0)));

	UINT numVertices = static_cast<UINT>(vertices.size());

	vector<DWORD> indices;
	for (UINT i = 0; i < numVertices - 2; i += 4)
	{
		indices.push_back(numVertices - 2);
		indices.push_back(i);
		indices.push_back((i + 4) % (numVertices - 2));
		indices.push_back(i + 1);
		indices.push_back(i + 2);
		indices.push_back((i + 6) % (numVertices - 2));
		indices.push_back((i + 6) % (numVertices - 2));
		indices.push_back((i + 5) % (numVertices - 2));
		indices.push_back(i + 1);
		indices.push_back(i + 3);
		indices.push_back(numVertices - 1);
		indices.push_back((i + 7) % (numVertices - 2));
	}

	m_NumIndices = static_cast<UINT>(indices.size());

	//Vertexbuffer
	D3D11_BUFFER_DESC bd{};
	D3D11_SUBRESOURCE_DATA initData{ 0 };

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = UINT(sizeof(VertexPosColNorm) * numVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = vertices.data();
	hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::GetInstance()->LogHResult(hr, L"Failed to Create Vertexbuffer");

	//Indexbuffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();
	hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::GetInstance()->LogHResult(hr, L"Failed to Create Indexbuffer");
}

void CylinderPosColorNorm::Update(const SceneContext& sceneContext)
{
}

void CylinderPosColorNorm::Draw(const SceneContext& sceneContext)
{
	DirectX::XMMATRIX world = XMLoadFloat4x4(&m_WorldMatrix);
	DirectX::XMMATRIX viewProj = XMLoadFloat4x4(&sceneContext.GetCamera()->GetViewProjection());
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

	// Render a torus
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, deviceContext);
		deviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}
