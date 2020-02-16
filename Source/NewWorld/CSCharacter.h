// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CSTypes.h"
#include "CSCharacter.generated.h"

// Forward Declaration
class ACSWeapon;

UCLASS()
class NEWWORLD_API ACSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACSCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAt(float Value);
	void LookUp(float Value);

	void SwitchWeapon(int32 index);
	
	void SpawnDefaultInventory();
	void DestoryInventory();
	void AddWeapon(ACSWeapon* InWeapon);
	void RemoveWeapon(ACSWeapon* InWeapon);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquipWeapon(ACSWeapon* NewWeapon);
	void EquipWeapon(ACSWeapon* NewWeapon);
	void SetCurrentWeapon(ACSWeapon* NewWeapon, ACSWeapon* LastWeapon);

	float BaseTurnRate;
	float BaseLookUpRate;

	UFUNCTION()
	void OnRep_Weapon(ACSWeapon* LastWeapon);

	FORCEINLINE const FCharacterAnimGraph& GetDefaultCharacterAnimGraph() { return DefaultCharacterAnimGraph; }

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Weapon)
	ACSWeapon* Weapon;

	UPROPERTY(Transient, Replicated)
	TArray<ACSWeapon*> Inventory;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	FCharacterAnimGraph DefaultCharacterAnimGraph;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TArray<TSubclassOf<ACSWeapon> > DefaultInventoryClasses;
};
