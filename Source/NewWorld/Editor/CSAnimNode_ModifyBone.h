// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include <BoneControllers/AnimNode_SkeletalControlBase.h>
#include <Curves/CurveVector.h>
#include "CSAnimNode_ModifyBone.generated.h"

/**
 * 
 */
struct FBoneContainer;
class ACSCharacter;

USTRUCT()
struct FBoneAndCurve
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FName BoneName;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UCurveVector> Curve;

	FBoneAndCurve()
		: BoneName(NAME_None)
		, Curve(nullptr)
	{}
};

struct FBoneReferenceAndCurve
{
	FBoneReference BoneToModify;
	TWeakObjectPtr<UCurveVector> Curve = nullptr;

	FBoneReferenceAndCurve(const FBoneReference& _BoneReference, UCurveVector* _Curve)
		: BoneToModify(_BoneReference)
		, Curve(_Curve)
	{}
};

USTRUCT()
struct NEWWORLD_API FCSAnimNode_ModifyBone : public FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()

	~FCSAnimNode_ModifyBone();
	
private:
	typedef Super FAnimNode_SkeletalControlBase;

	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	virtual void UpdateInternal(const FAnimationUpdateContext& Context) override;	

public:
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	void OnTakeDamage(class ACSCharacter* Victim, float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);
	void UpdateMaxTime();
	void UpdateHitDirection(AActor* Owner, AActor* Instigator);

public:
	UPROPERTY(EditAnywhere, Category = BoneAndCurve)
	TArray<FBoneAndCurve> CurveSettings;

private:
	const FBoneContainer* CachedRequiredBones = nullptr;
	TWeakObjectPtr<ACSCharacter> OwnerCharacter;
	TArray<FBoneReferenceAndCurve> BoneReferenceAndCurve;
	float fElapsedTime;
	float fMaxTime;
	bool m_bIsActive;
	float fFrontRate;
	float fSideRate;
	float fAngle;
	FDelegateHandle OnTakeDamageHandle;	
};
