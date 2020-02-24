// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CSTypes.h"
#include "CSAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType, meta = (BlueprintThreadSafe), Within = SkeletalMeshComponent)
class NEWWORLD_API UCSCharacterAnimInstance : public UAnimInstance
{
	GENERATED_UCLASS_BODY()

public:
	// the below functions are the native overrides for each phase
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;

	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	void UpdateCharacterLocomotion(float DeltaSeconds);

	UPROPERTY(Transient)
	class ACSCharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
	float Direction;

	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCharacterAnimGraph CharacterAnimGraph;

	UPROPERTY(BlueprintReadOnly)
	FCharacterAnimGraph PreviousCharacterAnimGraph;

	UPROPERTY(Transient, BlueprintReadWrite)
	float AnimGraphBlendAlpha = 1.f;
	float BlendInTime;
	float AccumulateBlendTime;

	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling;
};
