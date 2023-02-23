#pragma once
#include "GameObject.h"

class MeshData;
class DdsTexResource;

class MeshObjectTex final : public GameObject
{
public:
	MeshObjectTex(const std::wstring& assetFile, const std::wstring& textureFile);
	~MeshObjectTex() override;

	MeshObjectTex(const MeshObjectTex& t) = delete;
	MeshObjectTex& operator=(const MeshObjectTex& t) = delete;
	MeshObjectTex(const MeshObjectTex&& t) = delete;
	MeshObjectTex& operator=(const MeshObjectTex&& t) = delete;

	void Initialize() override;
	void Update(const SceneContext& sceneContext) override;
	void Draw(const SceneContext& sceneContext) override;

private:

	ID3D11InputLayout* m_pVertexLayout{};
	ID3D11Buffer* m_pVertexBuffer{};
	ID3D11Buffer* m_pIndexBuffer{};
	ID3DX11Effect* m_pEffect{};
	ID3DX11EffectTechnique* m_pTechnique{};
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable{};

	ID3DX11EffectShaderResourceVariable* m_pDiffuseVariable{};

	MeshData* m_pMeshData{};
	std::wstring m_AssetFile{}, m_TextureFile{};
	DdsTexResource* m_pTextureData{};
};
