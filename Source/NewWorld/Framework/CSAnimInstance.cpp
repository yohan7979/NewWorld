// Fill out your copyright notice in the Description page of Project Settings.


#include "CSAnimInstance.h"
#include "CSCharacter.h"
#include "CSWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"

UCSCharacterAnimInstance::UCSCharacterAnimInstance(const FObjectInitializer& ObjectInitializer)
{
	OwnerCharacter = nullptr;
}

void UCSCharacterAnimInstance::NativeInitializeAnimation()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (IsValid(OwnerPawn))
	{
		OwnerCharacter = Cast<ACSCharacter>(OwnerPawn);
	}

	Super::NativeInitializeAnimation();
}

void UCSCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(OwnerCharacter))
	{
		const FVector& CurrentVelocity = OwnerCharacter->GetVelocity();
		Direction = CalculateDirection(CurrentVelocity, OwnerCharacter->GetActorRotation());
		Speed = CurrentVelocity.Size();

		bIsFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();
		bIsStationary = Speed < KINDA_SMALL_NUMBER;

		UpdateCharacterLocomotion(DeltaSeconds);
	}

	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCSCharacterAnimInstance::UpdateCharacterLocomotion(float DeltaSeconds)
{
	// Get Weapon's CharacterAnimGraph or Defaults
	FCharacterAnimGraph DesiredAnimGraph = OwnerCharacter->GetDefaultCharacterAnimGraph();
	ACSWeapon* CurrentWeapon = OwnerCharacter->Weapon;
	if (IsValid(CurrentWeapon))
	{
		DesiredAnimGraph = CurrentWeapon->GetCharacterAnimGraph();
		DesiredAnimGraph.BlendInTime = CurrentWeapon->GetAnimGraphBlendTime();
	}

	// if weapon has changed, blend last to new.
	if (DesiredAnimGraph != CharacterAnimGraph)
	{
		PreviousCharacterAnimGraph = CharacterAnimGraph;
		CharacterAnimGraph = DesiredAnimGraph;

		BlendInTime = CharacterAnimGraph.BlendInTime;
		if (BlendInTime < KINDA_SMALL_NUMBER)
		{
			BlendInTime = 2.f; // Safe Defaults
		}

		AnimGraphBlendAlpha = 0.f;
		AccumulateBlendTime = 0.f;
	}

	AnimGraphBlendAlpha += ( (AccumulateBlendTime += DeltaSeconds) / BlendInTime );
	AnimGraphBlendAlpha = FMath::Clamp<float>(AnimGraphBlendAlpha, 0.f, 1.f);
}
