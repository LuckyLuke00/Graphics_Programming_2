#include "stdafx.h"
#include "SpherePosColorNorm.h"
#include "VertexStructs.h"
#include "ContentManager.h"

SpherePosColorNorm::SpherePosColorNorm(float radius, int slices, int stacks, DirectX::XMFLOAT4 color)
	: m_CustomColor{ color }
	, m_UseCustomColor{ true }
	, m_Radius{ radius }
	, m_Slices{ slices }
	, m_Stacks{ stacks }
{
}

SpherePosColorNorm::~SpherePosColorNorm()
{
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pVertexLayout);
}

void SpherePosColorNorm::Initialize()
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

	//**SPHERE**//
	std::vector<VertexPosColNorm> vertices;
	std::vector<unsigned int> indices;

	const float phiStep = 3.1415f / m_Stacks;

	// do not count the poles as rings
	const int numRings = m_Stacks - 1;

	// Compute vertices for each stack ring.
	for (int i = 1; i <= numRings; ++i)
	{
		const float phi = i * phiStep;

		// vertices of ring
		const float thetaStep = 2.0f * 3.1415f / float(m_Slices);
		for (int j = 0; j <= m_Slices; ++j)
		{
			const float theta = j * thetaStep;

			VertexPosColNorm v;

			// spherical to cartesian
			v.Position.x = m_Radius * sinf(phi) * cosf(theta);
			v.Position.y = m_Radius * cosf(phi);
			v.Position.z = m_Radius * sinf(phi) * sinf(theta);

			//// partial derivative of P with respect to theta
			//v.tangent.x = -radius*sinf(phi)*sinf(theta);
			//v.tangent.y = 0.0f;
			//v.tangent.z = radius*sinf(phi)*cosf(theta);

			//v.Normal = v.Position.Normalized();
			XMVECTOR vecNorm = XMVectorSet(v.Position.x, v.Position.y, v.Position.z, 0);
			vecNorm = XMVector3Normalize(vecNorm);
			XMStoreFloat3(&v.Normal, vecNorm);

			v.Color = m_CustomColor;
			//v.tex.x = theta / (2.0f*(float)D3DX_PI);
			//v.tex.y = phi / (float)D3DX_PI;

			vertices.push_back(v);
		}
	}

	// poles: note that there will be texture coordinate distortion
	vertices.push_back(VertexPosColNorm(DirectX::XMFLOAT3(0.0f, -m_Radius, 0.0f), m_CustomColor, DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)));
	vertices.push_back(VertexPosColNorm(DirectX::XMFLOAT3(0.0f, m_Radius, 0.0f), m_CustomColor, DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)));

	const int northPoleIndex = int(vertices.size() - 1);
	const int southPoleIndex = int(vertices.size() - 2);

	const int numRingVertices = m_Slices + 1;

	// Compute indices for inner stacks (not connected to poles).
	for (int i = 0; i < m_Stacks - 2; ++i)
	{
		for (int j = 0; j < m_Slices; ++j)
		{
			indices.push_back(i * numRingVertices + j);
			indices.push_back(i * numRingVertices + j + 1);
			indices.push_back((i + 1) * numRingVertices + j);

			indices.push_back((i + 1) * numRingVertices + j);
			indices.push_back(i * numRingVertices + j + 1);
			indices.push_back((i + 1) * numRingVertices + j + 1);
		}
	}

	// Compute indices for top stack.  The top stack was written
	// first to the vertex buffer.
	for (int i = 0; i < m_Slices; ++i)
	{
		indices.push_back(northPoleIndex);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	// Compute indices for bottom stack.  The bottom stack was written
	// last to the vertex buffer, so we need to offset to the index
	// of first vertex in the last ring.
	const int baseIndex = (numRings - 1) * numRingVertices;
	for (int i = 0; i < m_Slices; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}

	//Vertexbuffer
	D3D11_BUFFER_DESC bd{};
	D3D11_SUBRESOURCE_DATA initData{ 0 };

	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = UINT(sizeof(VertexPosColNorm) * vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = vertices.data();
	hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::GetInstance()->LogHResult(hr, L"Failed to Create Vertexbuffer");

	//Indexbuffer
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * static_cast<UINT>(indices.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = indices.data();
	hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::GetInstance()->LogHResult(hr, L"Failed to Create Indexbuffer");

	m_NumIndices = static_cast<int>(indices.size());
}

void SpherePosColorNorm::Update(const SceneContext& sceneContext)
{
}

void SpherePosColorNorm::Draw(const SceneContext& sceneContext)
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
