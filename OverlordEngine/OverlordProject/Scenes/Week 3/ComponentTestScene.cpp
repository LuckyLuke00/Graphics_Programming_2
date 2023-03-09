#include "stdafx.h"
#include "ComponentTestScene.h"

#include "Prefabs/SpherePrefab.h"

void ComponentTestScene::Initialize()
{
	auto& pPhysX{ PxGetPhysics() };
	auto pBounceyMat{ pPhysX.createMaterial(0.5f, 0.5f, 1.f) };

	// Ground Plane
	GameSceneExt::CreatePhysXGroundPlane(*this, pBounceyMat);

	// Sphere Red (Group 0)
	auto pSphere{ new SpherePrefab(1, 10, XMFLOAT4(Colors::Red)) };
	AddChild(pSphere);

	pSphere->GetTransform()->Translate(0.f, 30.f, 0.f);

	auto pRigidBody{ pSphere->AddComponent(new RigidBodyComponent(false)) };
	pRigidBody->AddCollider(PxSphereGeometry(1.f), *pBounceyMat);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);
	pRigidBody->SetCollisionIgnoreGroups(CollisionGroup::Group1 | CollisionGroup::Group2);

	// Sphere Green (Group 1)
	pSphere = new SpherePrefab(1, 10, XMFLOAT4(Colors::Green));
	AddChild(pSphere);

	pSphere->GetTransform()->Translate(0.5f, 20.f, 0.f);

	pRigidBody = pSphere->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxSphereGeometry(1.f), *pBounceyMat);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);
	pRigidBody->SetConstraint(RigidBodyConstraint::TransX | RigidBodyConstraint::TransZ, false);

	// Sphere Green (Group 2)
	pSphere = new SpherePrefab(1, 10, XMFLOAT4(Colors::Blue));
	AddChild(pSphere);

	pSphere->GetTransform()->Translate(0.f, 10.f, 0.f);

	pRigidBody = pSphere->AddComponent(new RigidBodyComponent(false));
	pRigidBody->AddCollider(PxSphereGeometry(1.f), *pBounceyMat);
	pRigidBody->SetCollisionGroup(CollisionGroup::Group2);
}
