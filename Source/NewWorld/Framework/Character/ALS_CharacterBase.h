// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ALS_CharacterBase.generated.h"

UENUM()
enum class EMovementState : uint8
{
	None,
	Ground,
	InAir,
	Ragdoll
};

UCLASS()
class NEWWORLD_API AALS_CharacterBase : public ACharacter
{
	GENERATED_UCLASS_BODY()

protected:	
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	void SetEssentialValues(float DeltaTime);
	void UpdateMovementMode(float DeltaTime);
	void CacheValues();

	void DoWhileOnGround(float DeltaTime);
	void DoWhileOnAir(float DeltaTime);
	void DoWhileOnRagdoll(float DeltaTime);

// 	void UpdateCharacterMovement();
// 	void UpdateGroundRotation(float DeltaTime);
// 	void UpdateInAirRotation(float DeltaTime);
// 	void RagdollUpdate(float DeltaTime);

protected:
	UPROPERTY(Transient)
	FVector Acceleration;

	UPROPERTY(Transient)
	float Speed;

	UPROPERTY(Transient)
	bool bIsMoving;

	UPROPERTY(Transient)
	FRotator LastVelocityRotation;

	UPROPERTY(Transient)
	float MovementInputAmount;

	UPROPERTY(Transient)
	bool bHasMovementInput;

	UPROPERTY(Transient)
	FRotator LastMovementInputRotation;

	UPROPERTY(Transient)
	float AimYawRate;

	UPROPERTY(Transient)
	float PreviousAimYawRate;

	UPROPERTY(Transient)
	FVector PreviousVelocity;

	UPROPERTY(Transient)
	EMovementState MovementState;
};
