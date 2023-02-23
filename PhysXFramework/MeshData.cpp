#include "stdafx.h"
#include "MeshData.h"
#include <algorithm>

MeshData::MeshData(std::vector<VertexBase> vertices, std::vector<DWORD> indices) :
	m_Vertices(vertices),
	m_Indices(indices),
	m_pVertexBuffer_PosNormCol{},
	m_pVertexBuffer_PosNormColTex{},
	m_pVertexBuffer_PosNormTex{},
	m_pIndexBuffer{}
{
}

MeshData::~MeshData()
{
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pVertexBuffer_PosNormCol);
	SafeRelease(m_pVertexBuffer_PosNormColTex);
	SafeRelease(m_pVertexBuffer_PosNormTex);
}

ID3D11Buffer* MeshData::GetIndexBuffer()
{
	if (m_pIndexBuffer)
		return m_pIndexBuffer;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * static_cast<UINT>(m_Indices.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = m_Indices.data();
	HRESULT hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::GetInstance()->LogHResult(hr, L"MeshData::GetIndexBuffer(...)> Failed to Create IndexBuffer");

	return m_pIndexBuffer;
}

ID3D11Buffer* MeshData::GetVertexBuffer_PosNormCol()
{
	if (m_pVertexBuffer_PosNormCol)
		return m_pVertexBuffer_PosNormCol;

	std::vector<VertexPosColNorm> verts;
	verts.reserve(m_Vertices.size());
	for_each(m_Vertices.begin(), m_Vertices.end(), [&verts](VertexBase& vertex)
		{
			verts.push_back(VertexPosColNorm(vertex));
		});

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosColNorm) * static_cast<UINT>(verts.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = verts.data();
	HRESULT hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer_PosNormCol);
	Logger::GetInstance()->LogHResult(hr, L"MeshData::GetVertexBuffer_PosNormCol(...)> Failed to Create VertexBuffer");

	return m_pVertexBuffer_PosNormCol;
}

ID3D11Buffer* MeshData::GetVertexBuffer_PosNormTex()
{
	if (m_pVertexBuffer_PosNormTex)
		return m_pVertexBuffer_PosNormTex;

	std::vector<VertexPosNormTex> verts;
	verts.reserve(m_Vertices.size());
	for_each(m_Vertices.begin(), m_Vertices.end(), [&verts](VertexBase& vertex)
		{
			verts.push_back(VertexPosNormTex(vertex));
		});

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosNormTex) * static_cast<UINT>(verts.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = verts.data();
	HRESULT hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer_PosNormTex);
	Logger::GetInstance()->LogHResult(hr, L"MeshData::GetVertexBuffer_PosNormCol(...)> Failed to Create VertexBuffer");

	return m_pVertexBuffer_PosNormTex;
}

ID3D11Buffer* MeshData::GetVertexBuffer_PosNormColTex()
{
	if (m_pVertexBuffer_PosNormColTex)
		return m_pVertexBuffer_PosNormColTex;

	std::vector<VertexPosColNormTex> verts;
	verts.reserve(m_Vertices.size());
	for_each(m_Vertices.begin(), m_Vertices.end(), [&verts](VertexBase& vertex)
		{
			verts.push_back(VertexPosColNormTex(vertex));
		});

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosColNormTex) * static_cast<UINT>(verts.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = verts.data();
	HRESULT hr = Locator::GetD3D11()->pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer_PosNormColTex);
	Logger::GetInstance()->LogHResult(hr, L"MeshData::GetVertexBuffer_PosNormColTex(...)> Failed to Create VertexBuffer");

	return m_pVertexBuffer_PosNormColTex;
}
