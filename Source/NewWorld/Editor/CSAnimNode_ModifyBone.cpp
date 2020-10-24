// copyrighted by swonlee ... twiclok@naver.com


#include "CSAnimNode_ModifyBone.h"
#include <BoneContainer.h>
#include "NewWorld/Framework/CSCharacter.h"
#include <Components/SkeletalMeshComponent.h>
#include "NewWorld/Framework/CSAnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include <GameFramework/Controller.h>
#include <GameFramework/DamageType.h>
#include <Engine/EngineTypes.h>

FCSAnimNode_ModifyBone::~FCSAnimNode_ModifyBone()
{
	if (OnTakeDamageHandle.IsValid())
	{
		ACSCharacter::OnTakeDamageEvent.Remove(OnTakeDamageHandle);
		OnTakeDamageHandle.Reset();
	}
}

void FCSAnimNode_ModifyBone::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	if (Context.AnimInstanceProxy)
	{
		UCSCharacterAnimInstance* MyAnimInstance = Cast<UCSCharacterAnimInstance>(Context.AnimInstanceProxy->GetAnimInstanceObject());
		if (MyAnimInstance)
		{
			ACSCharacter* CSCharacter = Cast<ACSCharacter>(MyAnimInstance->GetOwningActor());
			if (CSCharacter && CSCharacter != OwnerCharacter.Get()) 
			{
				OwnerCharacter = CSCharacter;
				OnTakeDamageHandle = ACSCharacter::OnTakeDamageEvent.AddRaw(this, &FCSAnimNode_ModifyBone::OnTakeDamage);
			}
		}
	}

	Super::Initialize_AnyThread(Context);
}

void FCSAnimNode_ModifyBone::UpdateInternal(const FAnimationUpdateContext& Context)
{
	if (nullptr == OwnerCharacter)
	{
		return;
	}

	if (false == m_bIsActive || BoneReferenceAndCurve.Num() == 0)
	{
		fElapsedTime = 0.f;
	}
	else
	{
		fElapsedTime += Context.GetDeltaTime();
		if (fElapsedTime > fMaxTime)
		{
			fElapsedTime = fMaxTime;
			m_bIsActive = false;
		}

		UE_LOG(LogTemp, Warning, TEXT("fElapsedTime : %f"), fElapsedTime);
	}
}

void FCSAnimNode_ModifyBone::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	CachedRequiredBones = &RequiredBones;
}

void FCSAnimNode_ModifyBone::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	check(OutBoneTransforms.Num() == 0);

	if (false == m_bIsActive)
	{
		return;
	}

	const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();
	for (const FBoneReferenceAndCurve& it : BoneReferenceAndCurve)
	{
		UCurveVector* pCurve = it.Curve.Get();
		if (pCurve == nullptr)
		{
			continue;
		}

		const FVector& CurveValue = pCurve->GetVectorValue(fElapsedTime);
		if (CurveValue.IsZero())
		{
			continue;
		}

		FCompactPoseBoneIndex CompactPoseBoneToModify = it.BoneToModify.GetCompactPoseIndex(BoneContainer);
		FTransform NewBoneTM = Output.Pose.GetComponentSpaceTransform(CompactPoseBoneToModify);
		FTransform ComponentTransform = Output.AnimInstanceProxy->GetComponentTransform();	
		FAnimationRuntime::ConvertCSTransformToBoneSpace(ComponentTransform, Output.Pose, NewBoneTM, CompactPoseBoneToModify, BCS_ComponentSpace);

		const FRotator CurveRotation(CurveValue.X, CurveValue.Y, CurveValue.Z);
		NewBoneTM.SetRotation(FQuat(CurveRotation) * NewBoneTM.GetRotation());

		FAnimationRuntime::ConvertBoneSpaceTransformToCS(ComponentTransform, Output.Pose, NewBoneTM, CompactPoseBoneToModify, BCS_ComponentSpace);
		Output.Pose.LocalBlendCSBoneTransforms(TArray<FBoneTransform> { FBoneTransform(it.BoneToModify.GetCompactPoseIndex(BoneContainer), NewBoneTM) }, 1.f);
		//OutBoneTransforms.Add(FBoneTransform(it.BoneToModify.GetCompactPoseIndex(BoneContainer), NewBoneTM));
	}
}

bool FCSAnimNode_ModifyBone::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	for (const FBoneReferenceAndCurve& it : BoneReferenceAndCurve)
	{
		if (false == it.BoneToModify.IsValidToEvaluate(RequiredBones))
		{
			return false;
		}
	}

	return true;
}

void FCSAnimNode_ModifyBone::OnTakeDamage(class ACSCharacter* Victim, float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (false == IsValid(Victim))
	{
		return;
	}

	ACSCharacter* CSCharacter = OwnerCharacter.Get();
	if (false == IsValid(CSCharacter))
	{
		return;
	}

	if (CSCharacter != Victim)
	{
		return;
	}

	if (false == IsValid(DamageCauser))
	{
		return;
	}

	const FVector& ForwardDir = CSCharacter->GetActorForwardVector();
	const FVector& TargetDir = (DamageCauser->GetActorLocation() - CSCharacter->GetActorLocation()).GetSafeNormal();

	BoneReferenceAndCurve.Empty();

	TArray<FBoneReferenceAndCurve> LocalBoneReferenceAndCurve;
	for (const FBoneAndCurve& BoneAndCurve : CurveSettings)
	{
		if (BoneAndCurve.BoneName.IsNone())
		{
			continue;
		}

		if (BoneAndCurve.Curve.IsNull())
		{
			continue;
		}

		LocalBoneReferenceAndCurve.Add(FBoneReferenceAndCurve(FBoneReference(BoneAndCurve.BoneName), BoneAndCurve.Curve.LoadSynchronous()));
	}

	for (int32 i = LocalBoneReferenceAndCurve.Num() - 1; i >= 0; --i)
	{
		if (false == LocalBoneReferenceAndCurve[i].BoneToModify.Initialize(*CachedRequiredBones))
		{
			LocalBoneReferenceAndCurve.RemoveAt(i);
		}
	}

	if (LocalBoneReferenceAndCurve.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Active true"));
		m_bIsActive = true;
		fElapsedTime = 0.f;

		BoneReferenceAndCurve = LocalBoneReferenceAndCurve;

		UpdateMaxTime();
	}
}

void FCSAnimNode_ModifyBone::UpdateMaxTime()
{
	fMaxTime = 0.f;
	float fMin, fMax;
	for (auto& it : BoneReferenceAndCurve)
	{		
		if (UCurveVector* pCurve = it.Curve.Get())
		{			
			pCurve->GetTimeRange(fMin, fMax);
			if (fMax > fMaxTime)
			{
				fMaxTime = fMax;
			}
		}
	}
}

