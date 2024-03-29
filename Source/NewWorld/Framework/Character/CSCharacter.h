// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NewWorld/Framework/CSTypes.h"
#include "NewWorld/Interface/CSSignificanceInterface.h"
#include "CSCharacter.generated.h"

// Forward Declaration
class ACSWeapon;

DECLARE_MULTICAST_DELEGATE_FiveParams(FOnTakeDamageEvent, class ACSCharacter* /*Victim*/, float /*Damage*/, struct FDamageEvent const& /*DamageEvent*/, AController* /*EventInstigator*/, AActor* /*DamageCauser*/)

UCLASS()
class NEWWORLD_API ACSCharacter : public ACharacter, public ICSSignificanceInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACSCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ** BEGIN **	SignificanceManager Interface
	virtual float SignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, const FTransform& ViewTransform);
	virtual void PostSignificanceFunction(USignificanceManager::FManagedObjectInfo* ObjectInfo, float OldSignificance, float Significance, bool bFinal);
	virtual FName GetObjectTag() const;
	// ** END **	SignificanceManager Interface

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAt(float Value);
	void LookUp(float Value);

	void DoJump(bool bPressed);
	void DoRoll(bool bPressed);
	void SetRollState(bool bRoll);
	bool CanRoll() const;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartRoll();

	UFUNCTION()
	void OnRep_IsRolling();

	void SetOrientRotationMode(bool bOrientRotationToMovement, float DelayTime = 0.f);
	void OnStartFire();
	void OnStopFire();
	virtual void PawnStartFire(uint8 FireModeNum = 0) override;
	void StopFire(const uint8 FireModeNum);

	void SwitchWeapon(int32 index);
	void OnInteract();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerInteract();

	AActor* GetNearestInteractActor();
	
	void SpawnDefaultWeaponInventory();
	void DestoryWeaponInventory();
	ACSWeapon* CreateAndGiveWeapon(const TSubclassOf<ACSWeapon>& WeaponClass);
	ACSWeapon* FindWeaponInventory(UClass* WeaponClass);
	void AddWeapon(ACSWeapon* InWeapon);
	void RemoveWeapon(ACSWeapon* InWeapon);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquipWeapon(ACSWeapon* NewWeapon);
	void EquipWeapon(ACSWeapon* NewWeapon);
	void SetCurrentWeapon(ACSWeapon* NewWeapon, ACSWeapon* LastWeapon);

	void SetCharacterStatus(const FCharacterStatus& InStatus);
	class UCSAttributeComponent* GetAttributeComponent() const { return AttributeComponent; }
	bool IsAliveAndWell() const;

	float BaseTurnRate;
	float BaseLookUpRate;

	DECLARE_EVENT_OneParam(ACSCharacter, FOnPressedJumpEvent, bool)
	FOnPressedJumpEvent& OnPressedJumpEvent() { return PressedJumpEvent; }

	static FOnTakeDamageEvent OnTakeDamageEvent;

	UFUNCTION()
	void OnRep_Weapon(ACSWeapon* LastWeapon);

	FORCEINLINE const FCharacterAnimGraph& GetDefaultCharacterAnimGraph() { return DefaultCharacterAnimGraph; }

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Weapon)
	ACSWeapon* Weapon;

	UPROPERTY(Transient, Replicated)
	TArray<ACSWeapon*> WeaponInventory;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* InteractionSphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCSAttributeComponent* AttributeComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FCharacterAnimGraph DefaultCharacterAnimGraph;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* RollAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TArray<TSubclassOf<ACSWeapon> > DefaultWeaponInventoryClasses;

	FTimerHandle TimerHandle_OrientRotationMode;
	FTimerHandle TimerHandle_RollEnd;

	FOnPressedJumpEvent PressedJumpEvent;

	UPROPERTY(ReplicatedUsing=OnRep_IsRolling)
	bool bIsRolling;

	UPROPERTY(ReplicatedUsing=OnRep_LastTakeHitInfo)
	FTakeHitInfo LastTakeHitInfo;

	UFUNCTION()
	void OnRep_LastTakeHitInfo();
	void ReplicateHit(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser, bool bKilled);

	/** notification when killed, for both the server and client. */
	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class ACSCharacter* PawnInstigator, class AActor* DamageCauser);

	/** play effects on hit */
	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class ACSCharacter* PawnInstigator, class AActor* DamageCauser);
};
