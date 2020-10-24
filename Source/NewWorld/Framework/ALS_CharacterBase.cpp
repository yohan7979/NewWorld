// copyrighted by swonlee ... twiclok@naver.com


#include "ALS_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

AALS_CharacterBase::AALS_CharacterBase(const FObjectInitializer& ObjectInitializer) :  Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AALS_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AALS_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetEssentialValues(DeltaTime);
	CacheValues();
}

void AALS_CharacterBase::SetEssentialValues(float DeltaTime)
{
	Acceleration = (GetVelocity() - PreviousVelocity) / DeltaTime;
	Speed = GetVelocity().Size2D();
	bIsMoving = Speed > 1.f;
	LastVelocityRotation = FRotationMatrix::MakeFromX(GetVelocity()).Rotator();
	MovementInputAmount = GetCharacterMovement()->GetCurrentAcceleration().Size() / GetCharacterMovement()->GetMaxAcceleration();
	bHasMovementInput = MovementInputAmount > 0.f;
	LastMovementInputRotation = FRotationMatrix::MakeFromX(GetCharacterMovement()->GetCurrentAcceleration()).Rotator();
	AimYawRate = FMath::Abs((GetControlRotation().Yaw - PreviousAimYawRate) / DeltaTime);

	UpdateMovementMode(DeltaTime);
}

void AALS_CharacterBase::UpdateMovementMode(float DeltaTime)
{
	switch (MovementState)
	{
	case EMovementState::Ground:
		DoWhileOnGround(DeltaTime);
		break;
	case EMovementState::InAir:
		DoWhileOnAir(DeltaTime);
		break;
	case EMovementState::Ragdoll:
		DoWhileOnRagdoll(DeltaTime);
		break;
	}
}

void AALS_CharacterBase::CacheValues()
{
	PreviousVelocity = GetVelocity();
	PreviousAimYawRate = GetControlRotation().Yaw;
}

void AALS_CharacterBase::DoWhileOnGround(float DeltaTime)
{
	//UpdateCharacterMovement();
	//UpdateGroundRotation(DeltaTime);
}

void AALS_CharacterBase::DoWhileOnAir(float DeltaTime)
{
	//UpdateInAirRotation(DeltaTime);
}

void AALS_CharacterBase::DoWhileOnRagdoll(float DeltaTime)
{
	//RagdollUpdate(DeltaTime);
}

