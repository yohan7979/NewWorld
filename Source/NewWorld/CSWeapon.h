// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSTypes.h"
#include "CSWeapon.generated.h"

class UAnimMontage;
class ACSCharacter;

namespace EWeaponState
{
	enum Type
	{
		Active,
		Inactive,
		Firing,
		Reloading,
		Equipping
	};
}

USTRUCT(BlueprintType)
struct FWeaponAnim
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Character3P;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Weapon3P;

	FWeaponAnim() 
		: Character3P(nullptr)
		, Weapon3P(nullptr) 
	{}
};

UCLASS(Abstract, NotBlueprintable)
class NEWWORLD_API ACSWeapon : public AActor
{
	GENERATED_UCLASS_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	virtual void OnEquip();
	virtual void OnUnEquip();
	virtual void OnEnterInventory(ACSCharacter* NewOwner);
	virtual void OnLeaveInventory();

	void SetOwningPawn(ACSCharacter* NewOwner);
	void SetCachedCharacter(AActor* NewOwner);
	virtual void OnRep_Owner() override;

	FORCEINLINE const FCharacterAnimGraph& GetCharacterAnimGraph() { return CharacterAnimGraph; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FCharacterAnimGraph CharacterAnimGraph;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<FWeaponAnim> FireAnims;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FWeaponAnim EquipAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FWeaponAnim UnEquipAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(Transient)
	ACSCharacter* CachedCharacter;
};
