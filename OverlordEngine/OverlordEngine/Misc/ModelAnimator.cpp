#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter) :
	m_pMeshFilter{ pMeshFilter }
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& /*sceneContext*/)
{
	TODO_W6_();

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1.
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)

		//2.
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount

		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB;
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount

		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		//Clear the m_Transforms vector
		//FOR every boneTransform in a key (So for every bone)
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector
	}
}

void ModelAnimator::SetAnimation(const std::wstring& /*clipName*/)
{
	TODO_W6_()
		//Set m_ClipSet to false
		//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
		//If found,
		//	Call SetAnimation(Animation Clip) with the found clip
		//Else
		//	Call Reset
		//	Log a warning with an appropriate message
}

void ModelAnimator::SetAnimation(UINT /*clipNumber*/)
{
	TODO_W6_()
		//Set m_ClipSet to false
		//Check if clipNumber is smaller than the actual m_AnimationClips vector size
		//If not,
			//	Call Reset
			//	Log a warning with an appropriate message
			//	return
		//else
			//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
			//	Call SetAnimation(AnimationClip clip)
}

void ModelAnimator::SetAnimation(const AnimationClip& /*clip*/)
{
	TODO_W6_()
		//Set m_ClipSet to true
		//Set m_CurrentClip

		//Call Reset(false)
}

void ModelAnimator::Reset(bool /*pause*/)
{
	TODO_W6_()
		//If pause is true, set m_IsPlaying to false

		//Set m_TickCount to zero
		//Set m_AnimationSpeed to 1.0f

		//If m_ClipSet is true
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		//Else
		//	Create an IdentityMatrix
		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
}
