#include "stdafx.h"
#include "MeshDataLoader.h"
#include "BinaryReader.h"
#include <algorithm>
#include "Logger.h"
MeshData* MeshDataLoader::LoadContent(const std::wstring& assetFile)
{
	BinaryReader binReader(assetFile);

	if (!binReader.Exists())
		return nullptr;

	vector<VertexBase> vertices;
	vector<DWORD> indices;

	//READ OVM FILE
	int versionMajor = binReader.Read<char>();
	int versionMinor = binReader.Read<char>();

	if (versionMajor != 1 || versionMinor != 1)
	{
		wstringstream ss;
		ss << L"MeshDataLoader::Load() > Wrong OVM version\nFile: ";
		ss << assetFile;
		ss << L"\nExpected version 1.1, not " << versionMajor << L"." << versionMinor;
		Logger::GetInstance()->LogWarning(ss.str());

		return nullptr;
	}

	UINT vertexCount = 0;
	UINT indexCount = 0;
	MeshDataType meshDataType{ MeshDataType::END };
	do
	{
		meshDataType = (MeshDataType)binReader.Read<char>();

		auto dataOffset = binReader.Read<UINT>();

		switch (meshDataType)
		{
			//HEADER
		case MeshDataType::HEADER:
		{
			auto name = binReader.ReadString();
			Logger::GetInstance()->LogDebug(wstring(L"Parsing mesh: ") + name);
			vertexCount = binReader.Read<UINT>();
			indexCount = binReader.Read<UINT>();
			vertices.resize(vertexCount);
			indices.resize(indexCount);
		}
		break;
		case MeshDataType::POSITIONS:
		{
			for (UINT i = 0; i < vertexCount; ++i)
			{
				vertices[i].Position.x = binReader.Read<float>();
				vertices[i].Position.y = binReader.Read<float>();
				vertices[i].Position.z = binReader.Read<float>();
			}
		}
		break;
		case MeshDataType::INDICES:
		{
			for (UINT i = 0; i < indexCount; ++i)
			{
				indices[i] = binReader.Read<DWORD>();
			}
		}
		break;
		case MeshDataType::NORMALS:
		{
			for_each(vertices.begin(), vertices.end(), [&binReader](VertexBase& vertex)
				{
					vertex.Normal.x = binReader.Read<float>();
					vertex.Normal.y = binReader.Read<float>();
					vertex.Normal.z = binReader.Read<float>();
				});
		}
		break;
		case MeshDataType::TEXCOORDS:
		{
			for_each(vertices.begin(), vertices.end(), [&binReader](VertexBase& vertex)
				{
					vertex.TexCoord.x = binReader.Read<float>();
					vertex.TexCoord.y = binReader.Read<float>();
				});
		}
		break;
		case MeshDataType::COLORS:
		{
			for_each(vertices.begin(), vertices.end(), [&binReader](VertexBase& vertex)
				{
					vertex.Color.x = binReader.Read<float>();
					vertex.Color.y = binReader.Read<float>();
					vertex.Color.z = binReader.Read<float>();
					vertex.Color.w = binReader.Read<float>();
				});
		}
		break;
		default:
			binReader.MoveBufferPosition(dataOffset);
			break;
		}
	} while (meshDataType != MeshDataType::END);

	return new MeshData(vertices, indices);
}

void MeshDataLoader::Destroy(MeshData* pObjToDestroy)
{
	SafeDelete(pObjToDestroy);
}
