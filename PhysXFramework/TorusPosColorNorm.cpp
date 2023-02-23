#include "stdafx.h"
#include "TorusPosColorNorm.h"
#include "VertexStructs.h"
#include "ContentManager.h"
#include "Logger.h"
#include <vector>

TorusPosColorNorm::TorusPosColorNorm(float majorRadius, UINT majorRadiusSteps, float minorRadius, UINT minorRadiusSteps, DirectX::XMFLOAT4 color)
	: m_MajorRadius{ majorRadius }
	, m_MinorRadius{ minorRadius }
	, m_MajorRadiusSteps{ majorRadiusSteps }
	, m_MinorRadiusSteps{ minorRadiusSteps }
	, m_Color{ color }
{
}

TorusPosColorNorm::~TorusPosColorNorm()
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pVertexLayout);
}

void TorusPosColorNorm::Initialize()
{
	//Effect
	m_pEffect = ContentManager::GetInstance()->Load<ID3DX11Effect>(L"Resources/Effects/PosColNorm3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pMatWorldViewProjVariable->IsValid())Logger::GetInstance()->LogWarning(L"TorusPosColorNorm::Initialize() - WvpVariable is invalid!");
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	if (!m_pMatWorldVariable->IsValid())Logger::GetInstance()->LogWarning(L"TorusPosColorNorm::Initialize() - WorldVariable is invalid!");

	//InputLayout
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = static_cast<UINT>(std::size(vertexDesc));

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

	//**TORUS**

	//Vertices
	vector<VertexPosColNorm> vertices;
	float majorInterval = XM_2PI / m_MajorRadiusSteps;
	float minorInterval = XM_2PI / m_MinorRadiusSteps;
	for (UINT majorStep = 0; majorStep < m_MajorRadiusSteps; ++majorStep)
	{
		float torusRadius = majorStep * majorInterval;

		for (UINT minorStep = 0; minorStep < m_MinorRadiusSteps; ++minorStep)
		{
			float circleRadius = minorStep * minorInterval;
			DirectX::XMFLOAT3 pos;
			pos.x = (m_MajorRadius + (m_MinorRadius * cos(circleRadius))) * cos(torusRadius);
			pos.y = (m_MajorRadius + (m_MinorRadius * cos(circleRadius))) * sin(torusRadius);
			pos.z = m_MinorRadius * sin(circleRadius);

			//Normal
			DirectX::XMFLOAT3 middle;
			middle.x = m_MajorRadius * cos(torusRadius);
			middle.y = m_MajorRadius * sin(torusRadius);
			middle.z = 0;

			XMVECTOR vecPos = XMLoadFloat3(&pos);
			XMVECTOR vecMiddle = XMLoadFloat3(&middle);
			XMVECTOR vecNormal = XMVector3Normalize(XMVectorSubtract(vecPos, vecMiddle));
			DirectX::XMFLOAT3 normal;
			XMStoreFloat3(&normal, vecNormal);

			vertices.push_back(VertexPosColNorm(pos, m_Color, normal));
		}
	}

	UINT numVerts = static_cast<UINT>(vertices.size());

	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosColNorm) * numVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = vertices.data();
	hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::GetInstance()->LogHResult(hr, L"Failed to Create Vertexbuffer");

	//Indices
	vector<DWORD> indices;
	for (DWORD i = 0; i < static_cast<DWORD>(numVerts); ++i)
	{
		DWORD v0, v1, v2, v3;
		v0 = i;
		v1 = (v0 + m_MinorRadiusSteps) % numVerts;
		v2 = v1 + 1;
		v3 = i + 1;

		if ((i + 1) % m_MinorRadiusSteps == 0)
		{
			v2 -= m_MinorRadiusSteps;
			v3 -= m_MinorRadiusSteps;
		}

		indices.push_back(v0);
		indices.push_back(v1);
		indices.push_back(v2);
		indices.push_back(v2);
		indices.push_back(v3);
		indices.push_back(v0);
	}

	m_NumIndices = static_cast<UINT>(indices.size());

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();
	hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::GetInstance()->LogHResult(hr, L"Failed to Create Indexbuffer");
}

void TorusPosColorNorm::Update(const SceneContext& sceneContext)
{
}

void TorusPosColorNorm::Draw(const SceneContext& sceneContext)
{
	DirectX::XMMATRIX world = XMLoadFloat4x4(&m_WorldMatrix);
	const DirectX::XMMATRIX viewProj = XMLoadFloat4x4(&sceneContext.GetCamera()->GetViewProjection());
	DirectX::XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));

	// Set vertex buffer
	constexpr UINT stride = sizeof(VertexPosColNorm);
	constexpr UINT offset = 0;
	const auto deviceContext = Locator::GetD3D11()->pDeviceContext;
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
