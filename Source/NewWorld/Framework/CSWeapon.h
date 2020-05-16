// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSTypes.h"
#include "CSWeaponTypes.h"
#include "CSWeapon.generated.h"

class UAnimMontage;
class ACSCharacter;
class UCSWeaponState;

UENUM(BlueprintType)
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

USTRUCT()
struct FAttachPoint
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName GripSocketName;

	UPROPERTY(EditDefaultsOnly)
	FName UnEquipSocketName;

	FAttachPoint()
		: GripSocketName(NAME_None)
		, UnEquipSocketName(NAME_None)
	{}
};

UCLASS(Abstract, NotBlueprintable)
class NEWWORLD_API ACSWeapon : public AActor
{
	GENERATED_UCLASS_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	virtual void OnEquip();
	virtual void OnUnEquip();
	virtual void OnEnterWeaponInventory(ACSCharacter* NewOwner);
	virtual void OnLeaveWeaponInventory();

	void AttachMeshToCharacter(bool bEquip);
	void DetachMeshFromCharacter();
	void SetVisibility(bool bVisible);
	UPrimitiveComponent* GetMeshComponent();

	void SetOwningPawn(ACSCharacter* NewOwner);
	void SetCachedCharacter(AActor* NewOwner);
	virtual void OnRep_Owner() override;

	virtual void StartFire(const uint8 FireModeNum);
	virtual void StopFire(const uint8 FireModeNum);

	virtual bool CanFire();
	virtual bool IsFiring() const;
	virtual bool IsValidStateToFire() const;

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerStartFire(const uint8 FireModeNum);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerStopFire(const uint8 FireModeNum);

	void SetWeaponState(EWeaponState::Type NewState);
	void OnWeaponStateChanged(uint8 OldState, uint8 NewState);
	void PlayCurrentStateAnimation();
	virtual float GetStateTransitionTime() const;
	virtual void BringUpFinished();
	virtual void PutDown();
	virtual void FireWeapon(bool bWantsToFire);

	virtual void PlayFiringAnimation(const int32 FireIndex);
	virtual void PlayWeaponAnimation(const FWeaponAnim& InWeaponAnim);
	virtual FVector GetCameraStartLocation(const FVector& AimDir) const;
	virtual FVector GetAdjustedAim() const;
	virtual FVector GetMuzzleLocation(const FVector& AimDir, const int32 ComboCount = 0) const;
	
	bool WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, FHitResult& OutHit);

	bool IsLocallyControlled();

	UFUNCTION()
	void OnRep_CurrentState(uint8 OldState);

	FORCEINLINE const FCharacterAnimGraph& GetCharacterAnimGraph() { return CharacterAnimGraph; }
	FORCEINLINE const float GetAnimGraphBlendTime() { return AnimGraphBlendOutTime; }
	FORCEINLINE FName GetMuzzleSocketName() const { return MuzzleSocketName; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FCharacterAnimGraph CharacterAnimGraph;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<FWeaponAnim> FireAnims;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FWeaponAnim EquipAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	FWeaponAnim UnEquipAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float AnimGraphBlendOutTime = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Attachment")
	FAttachPoint AttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float EquipTime;

	UPROPERTY(EditDefaultsOnly, Category = "State")
	float ReloadTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkelMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(Transient)
	ACSCharacter* CachedCharacter;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentState)
	uint8 CurrentState;

	UPROPERTY(Transient)
	UCSWeaponState* CurrentStateNode;

	typedef TMap<TEnumAsByte<EWeaponState::Type>, TSubclassOf<UCSWeaponState> > FCSWeaponStateClassMap;
	FCSWeaponStateClassMap WeaponStateClassMap;

	UPROPERTY(Transient)
	TMap<TEnumAsByte<EWeaponState::Type>, UCSWeaponState*> WeaponStateMap;

	UPROPERTY(EditDefaultsOnly, Instanced)
	class UCSWeaponFiringAction* FiringAction;

	UPROPERTY(EditDefaultsOnly)
	FInstantWeaponConfig InstantWeaponConfig;

	UPROPERTY(EditDefaultsOnly)
	FName MuzzleSocketName;
};
