// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CSCharacter.h"
#include "CSEquipableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API ACSEquipableCharacter : public ACSCharacter
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:
	UFUNCTION()
	void OnRep_AttachedSkelMeshes();

	virtual void SetAttachedSkeletalMesh(USkeletalMesh* MeshToAttach, int32 SlotIndex);
	USkeletalMeshComponent* GetSkeletalMeshByType(EEquipmentSlot SlotType) const;
	
public:
	// ** Default SkeletalMeshComponent Begin
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Body_Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Body_Chest;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Body_Hands;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Body_Legs;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Body_Feet;
	// ** Default SkeletalMeshComponent End

	// ** Equip SkeletalMeshComponent Begin
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Head;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Shoulder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Chest;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Hands;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Legs;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Feet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Back;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Waist;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Accessory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Earing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Ring;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Trinket;
	// ** Equip SkeletalMeshComponent End

	UPROPERTY(ReplicatedUsing=OnRep_AttachedSkelMeshes)
	USkeletalMesh* AttachedSkelMeshes[EEquipmentSlot::MAX];

	UPROPERTY(EditDefaultsOnly)
	TMap<EEquipmentSlot, class USkeletalMesh*> DefaultBodyMeshMap;
};
