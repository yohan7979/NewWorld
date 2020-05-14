// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/CSCharacter.h"
#include "CSEquipableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API ACSEquipableCharacter : public ACSCharacter
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION()
	void OnRep_AttachedSkelMeshes();

	virtual void SetAttachedSkeletalMesh(USkeletalMesh* MeshToAttach, int32 SlotIndex);
	USkeletalMeshComponent* GetSkeletalMeshByType(EEquipmentSlot SlotType) const;

public:
	// SkeletalMeshComponent Begin
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Shoulder;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Chest;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Hands;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Legs;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Feet;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Back;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Waist;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Accessory;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Earing;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Ring;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Trinket;
	// SkeletalMeshComponent End

	UPROPERTY(ReplicatedUsing=OnRep_AttachedSkelMeshes)
	USkeletalMesh* AttachedSkelMeshes[EEquipmentSlot::MAX];
};
