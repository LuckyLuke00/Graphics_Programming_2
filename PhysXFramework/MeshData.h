#pragma once
#include "VertexStructs.h"
#include "Logger.h"

class MeshData
{
public:
	MeshData(std::vector<VertexBase> vertices, std::vector<DWORD> indices);
	~MeshData();
	MeshData(const MeshData& obj) = delete;
	MeshData& operator=(const MeshData& obj) = delete;

	UINT GetVertexCount() const { return static_cast<UINT>(m_Vertices.size()); }
	UINT GetIndexCount() const { return static_cast<UINT>(m_Indices.size()); }

	ID3D11Buffer* GetIndexBuffer();

	template<typename T>
	ID3D11Buffer* GetVertexBuffer()
	{
		const type_info& type = typeid(T);
		if (type == typeid(VertexPosColNorm))
			return GetVertexBuffer_PosNormCol();

		if (type == typeid(VertexPosColNormTex))
			return GetVertexBuffer_PosNormColTex();

		if (type == typeid(VertexPosNormTex))
			return GetVertexBuffer_PosNormTex();

		std::wstringstream ss;
		ss << L"MeshData::GetVertexBuffer<" << type.name() << L">(): No implementation found for \'" << type.name() << L"\'!";
		Logger::GetInstance()->LogWarning(ss.str());
		return nullptr;
	}

private:

	ID3D11Buffer* m_pIndexBuffer;

	ID3D11Buffer* m_pVertexBuffer_PosNormCol;
	ID3D11Buffer* GetVertexBuffer_PosNormCol();

	ID3D11Buffer* m_pVertexBuffer_PosNormColTex;
	ID3D11Buffer* GetVertexBuffer_PosNormColTex();

	ID3D11Buffer* m_pVertexBuffer_PosNormTex;
	ID3D11Buffer* GetVertexBuffer_PosNormTex();

	std::vector<VertexBase> m_Vertices;
	std::vector<DWORD> m_Indices;
};
