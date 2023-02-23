#pragma once
#include "GameObject.h"

class MeshData;

class MeshObject final : public GameObject
{
public:
	MeshObject(const std::wstring& assetFile);
	~MeshObject() override;

	MeshObject(const MeshObject& other) = delete;
	MeshObject& operator=(const MeshObject& other) = delete;
	MeshObject(const MeshObject&& other) = delete;
	MeshObject& operator=(const MeshObject&& other) = delete;

	void Initialize() override;
	void Update(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& sceneContext) override;

private:
	MeshData* m_pMeshData{};
	std::wstring m_AssetFile{};

	ID3D11InputLayout* m_pVertexLayout{};
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	ID3DX11Effect* m_pEffect{};
	ID3DX11EffectTechnique* m_pTechnique{};
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable{};
};
