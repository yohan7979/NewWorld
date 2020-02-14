// Fill out your copyright notice in the Description page of Project Settings.


#include "CSAnimInstance.h"
#include "CSCharacter.h"
#include "CSWeapon.h"

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
	if (OwnerCharacter)
	{
		const FVector& CurrentVelocity = OwnerCharacter->GetVelocity();
		Direction = CalculateDirection(CurrentVelocity, OwnerCharacter->GetActorRotation());
		Speed = CurrentVelocity.Size();

		//@TODO : Get Weapon's CharacterAnimGraph
		CharacterAnimGraph = OwnerCharacter->GetDefaultCharacterAnimGraph();
	}

	Super::NativeUpdateAnimation(DeltaSeconds);
}
