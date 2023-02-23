#include "stdafx.h"
#include "GameObject.h"
#include "D3D11.h"
#include "GameScene.h"
#include "Logger.h"

GameObject::GameObject() :
	m_pScene{ nullptr },
	m_pParent{ nullptr },
	m_IsInitialized{ false },
	m_WorldMatrix{ DirectX::XMFLOAT4X4{} },
	m_Position{ DirectX::XMFLOAT3{0,0,0} },
	m_Scale{ DirectX::XMFLOAT3{1,1,1} },
	m_Rotation{ DirectX::XMFLOAT4{0,0,0,1} },
	m_WorldPosition{ DirectX::XMFLOAT3{0,0,0} },
	m_WorldScale{ DirectX::XMFLOAT3{1,1,1} },
	m_WorldRotation{ DirectX::XMFLOAT4{0,0,0,1} },
	m_pRigidActor{ nullptr },
	m_PositionChanged{ false },
	m_RotationChanged{ false },
	m_ScaleChanged{ false },
	m_RigidActorAdded{ false },
	m_CanDraw{ true }
{
	XMStoreFloat4x4(&m_WorldMatrix, DirectX::XMMatrixIdentity());
}

GameObject::~GameObject()
{
	for (GameObject* pObject : m_Children)
	{
		SafeDelete(pObject);
	}
}

GameScene* GameObject::GetScene() const
{
	if (m_pScene != nullptr)
		return m_pScene;

	if (m_pParent != nullptr)
		return m_pParent->GetScene();

	return nullptr;
}

void GameObject::AddChild(GameObject* pObject)
{
	auto it = std::find(m_Children.begin(), m_Children.end(), pObject);
	if (it == m_Children.end())
	{
		//Detach from possible previous parent
		if (pObject->GetParent())
		{
			pObject->GetParent()->RemoveChild(pObject);
		}

		//If attached to scene, also detach
		if (pObject->GetScene())
		{
			pObject->GetScene()->RemoveGameObject(pObject);
		}

		m_Children.push_back(pObject);
		pObject->m_pScene = nullptr;
		pObject->m_pParent = this;

		if (m_IsInitialized)
			pObject->RootInitialize();
	}
}

void GameObject::AttachRigidActor(PxRigidActor* pRigidActor)
{
	m_RigidActorAdded = false;
	if (m_pRigidActor != nullptr)
	{
		m_pRigidActor->release();
		Logger::GetInstance()->LogWarning(L"GameObject::AttachRigidActor(...) > Overwriting current Actor! (Previous actor released)");
	}

	m_pRigidActor = pRigidActor;
	m_pRigidActor->userData = this;

	if (m_IsInitialized)
	{
		//Attach RigidBody
		if (m_pRigidActor)
		{
			PxTransform transform;
			transform.p = PxVec3(m_WorldPosition.x, m_WorldPosition.y, m_WorldPosition.z);
			transform.q = PxQuat(m_WorldRotation.x, m_WorldRotation.y, m_WorldRotation.z, m_WorldRotation.w);
			m_pRigidActor->setGlobalPose(transform);
			PxScaleRigidActor(*m_pRigidActor, m_Scale.x);

			//Attach actor to physx scene
			auto pScene = GetScene();
			if (pScene)
			{
				pScene->GetPhysxScene()->addActor(*m_pRigidActor);
				m_RigidActorAdded = true;
			}
		}
	}
}

PxRigidActor* GameObject::DetachRigidActor()
{
	// don't delete the actor, just detach it from the GameObject
	auto pActor = m_pRigidActor;
	m_pRigidActor->userData = nullptr;
	m_pRigidActor = nullptr;
	m_RigidActorAdded = false;
	return pActor; // move ownership to the caller (!)
}

bool GameObject::RemoveChild(GameObject* pObject, bool deleteObject)
{
	// do not erase the object, just remove it from the graph
	auto it = std::find(m_Children.begin(), m_Children.end(), pObject);
	if (it != m_Children.end())
	{
		m_Children.erase(it);
		pObject->m_pScene = nullptr;
		pObject->m_pParent = nullptr;

		if (deleteObject)
			SafeDelete(pObject);

		return true;
	}

	return false;
}

void GameObject::Translate(float x, float y, float z)
{
	if (m_RigidActorAdded)
	{
		if (m_pRigidActor->getType() == PxActorType::eRIGID_STATIC)
		{
			Logger::GetInstance()->LogWarning(L"GameObject::Translate(...) > Can't translate a STATIC RigidActor!");
			return;
		}

		m_PositionChanged = true;
	}

	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void GameObject::Rotate(float x, float y, float z)
{
	if (m_RigidActorAdded)
	{
		if (m_pRigidActor->getType() == PxActorType::eRIGID_STATIC)
		{
			Logger::GetInstance()->LogWarning(L"GameObject::Rotate(...) > Can't rotate a STATIC RigidActor!");
			return;
		}

		m_RotationChanged = true;
	}

	XMStoreFloat4(&m_Rotation, XMQuaternionRotationRollPitchYaw(x, y, z));
}

void GameObject::RotateDegrees(float x, float y, float z)
{
	if (m_RigidActorAdded)
	{
		if (m_pRigidActor->getType() == PxActorType::eRIGID_STATIC)
		{
			Logger::GetInstance()->LogWarning(L"GameObject::Rotate(...) > Can't rotate a STATIC RigidActor!");
			return;
		}

		m_RotationChanged = true;
	}

	XMStoreFloat4(&m_Rotation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(x), XMConvertToRadians(y), XMConvertToRadians(z)));
}

void GameObject::Scale(float x, float y, float z)
{
	if (m_RigidActorAdded)
	{
		if (m_pRigidActor->getType() == PxActorType::eRIGID_STATIC)
		{
			Logger::GetInstance()->LogWarning(L"GameObject::Scale(...) > Can't scale a STATIC RigidActor!");
			return;
		}

		if (x == y && y == z)
		{
			m_ScaleChanged = true;
		}
		else
		{
			Logger::GetInstance()->LogWarning(L"GameObject::Scale(...) > Only Uniform scales are allowed (RigidActor)!");
			return;
		}
	}

	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

void GameObject::RootInitialize()
{
	if (m_IsInitialized)	return;

	Initialize();

	for (GameObject* pObject : m_Children)
	{
		pObject->RootInitialize();
	}

	//Set Initial Pose. (Only update a Root-GameObject (Children are updated by parent) => m_Parent == nullptr)
	if (!m_pParent)
	{
		CalculateWorldMatrix(DirectX::XMMatrixIdentity());
	}

	//Attach RigidBody
	if (m_pRigidActor)
	{
		PxTransform transform;
		transform.p = PxVec3(m_WorldPosition.x, m_WorldPosition.y, m_WorldPosition.z);
		transform.q = PxQuat(m_WorldRotation.x, m_WorldRotation.y, m_WorldRotation.z, m_WorldRotation.w);
		m_pRigidActor->setGlobalPose(transform);
		PxScaleRigidActor(*m_pRigidActor, m_Scale.x);

		//Attach actor to physx scene
		auto scene = GetScene();
		if (scene)
		{
			scene->GetPhysxScene()->addActor(*m_pRigidActor);
			m_RigidActorAdded = true;
		}
	}

	m_IsInitialized = true;
}

void GameObject::RootUpdate(const SceneContext& sceneContext)
{
	Update(sceneContext);

	for (GameObject* pObject : m_Children)
	{
		pObject->RootUpdate(sceneContext);
	}
}

void GameObject::RootDraw(const SceneContext& sceneContext)
{
	if (!m_CanDraw)
		return;

	Draw(sceneContext);

	for (GameObject* pObject : m_Children)
	{
		pObject->RootDraw(sceneContext);
	}
}

void GameObject::CalculateWorldMatrix(const DirectX::XMMATRIX& parentWorld, bool updateChildren)
{
	if (m_RigidActorAdded)
	{
		bool isKinematic = m_pRigidActor->getType() == PxActorType::eRIGID_DYNAMIC && (((PxRigidDynamic*)m_pRigidActor)->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC));

		PxTransform globalPose = m_pRigidActor->getGlobalPose();

		if (m_PositionChanged)
		{
			globalPose.p.x = m_Position.x;
			globalPose.p.y = m_Position.y;
			globalPose.p.z = m_Position.z;
			if (!isKinematic)((PxRigidDynamic*)m_pRigidActor)->setLinearVelocity(PxVec3(0.f, 0.f, 0.f));
		}
		else
		{
			m_Position.x = globalPose.p.x;
			m_Position.y = globalPose.p.y;
			m_Position.z = globalPose.p.z;
		}

		if (m_RotationChanged)
		{
			globalPose.q.x = m_Rotation.x;
			globalPose.q.y = m_Rotation.y;
			globalPose.q.z = m_Rotation.z;
			globalPose.q.w = m_Rotation.w;
			if (!isKinematic)((PxRigidDynamic*)m_pRigidActor)->setAngularVelocity(PxVec3(0.f, 0.f, 0.f));
		}
		else
		{
			m_Rotation.x = globalPose.q.x;
			m_Rotation.y = globalPose.q.y;
			m_Rotation.z = globalPose.q.z;
			m_Rotation.w = globalPose.q.w;
		}

		if (m_PositionChanged || m_RotationChanged)
		{
			if (isKinematic)((PxRigidDynamic*)m_pRigidActor)->setKinematicTarget(globalPose);
			else m_pRigidActor->setGlobalPose(globalPose);
		}

		if (m_ScaleChanged)
		{
			PxScaleRigidActor(*m_pRigidActor, m_Scale.x);
		}
	}

	m_PositionChanged = m_RotationChanged = m_ScaleChanged = false;
	XMMATRIX matTrans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	XMMATRIX matRot = XMMatrixRotationQuaternion(XMLoadFloat4(&m_Rotation));
	XMMATRIX matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	XMMATRIX matWorld = XMMatrixMultiply(matScale, matRot);
	matWorld = XMMatrixMultiply(matWorld, matTrans);
	matWorld = XMMatrixMultiply(matWorld, parentWorld);

	XMStoreFloat4x4(&m_WorldMatrix, matWorld);

	//Retrieve World Transform Information
	XMVECTOR outScale, outRotation, outPosition;
	XMMatrixDecompose(&outScale, &outRotation, &outPosition, matWorld);
	XMStoreFloat3(&m_WorldPosition, outPosition);
	XMStoreFloat3(&m_WorldScale, outScale);
	XMStoreFloat4(&m_WorldRotation, outRotation);

	if (!updateChildren)
		return;

	for (GameObject* pObject : m_Children)
	{
		pObject->CalculateWorldMatrix(matWorld, updateChildren);
	}
}
