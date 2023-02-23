#pragma once

#include "D3D11.h"
#include "SceneContext.h"
#include "PhysxManager.h"
#include <vector>

class GameScene;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();
	GameObject(const GameObject& t) = delete;
	GameObject& operator=(const GameObject& t) = delete;

	const XMFLOAT3& GetPosition() const { return m_Position; }
	const XMFLOAT3& GetScale() const { return m_Scale; }
	const XMFLOAT4& GetRotation() const { return m_Rotation; }
	const XMFLOAT4X4& GetWorldMatrix() const { return m_WorldMatrix; }

	GameScene* GetScene() const;
	GameObject* GetParent() const { return m_pParent; }
	PxRigidActor* GetRigidActor() const { return m_pRigidActor; }

	void AddChild(GameObject* pObject);
	void AttachRigidActor(PxRigidActor* pRigidActor);
	PxRigidActor* DetachRigidActor();
	bool RemoveChild(GameObject* pObject, bool deleteObject = false);

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void RotateDegrees(float x, float y, float z);
	void Scale(float x, float y, float z);
	void CanDraw(bool canDraw) { m_CanDraw = canDraw; }

	virtual void Initialize() = 0;
	virtual void Draw(const SceneContext& sceneContext) = 0;
	virtual void Update(const SceneContext& sceneContext) = 0;

protected:

	bool m_IsInitialized;
	std::vector<GameObject*> m_Children;

	DirectX::XMFLOAT4X4 m_WorldMatrix;
	DirectX::XMFLOAT3 m_Position, m_Scale;
	DirectX::XMFLOAT4 m_Rotation;
	DirectX::XMFLOAT3 m_WorldPosition, m_WorldScale;
	DirectX::XMFLOAT4 m_WorldRotation;
	PxRigidActor* m_pRigidActor;

private:
	friend GameScene;

	void RootInitialize();
	void RootUpdate(const SceneContext& sceneContext);
	void RootDraw(const SceneContext& sceneContext);
	void CalculateWorldMatrix(const DirectX::XMMATRIX& parentWorld, bool updateChildren = true);

	GameScene* m_pScene;
	GameObject* m_pParent;
	bool m_PositionChanged, m_RotationChanged, m_ScaleChanged, m_RigidActorAdded;
	bool m_CanDraw;
};
