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
	USkeletalMeshComponent* HeadSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* ShoulderSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* ChestSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* HandsSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* LegsSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* FeetSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* BackSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* WaistSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* AccessorySkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* EaringSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* RingSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* TrinketSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* MainHandSkelMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* OffHandSkelMesh;
	// SkeletalMeshComponent End

	UPROPERTY(ReplicatedUsing=OnRep_AttachedSkelMeshes)
	USkeletalMesh* AttachedSkelMeshes[EEquipmentSlot::MAX];
};
