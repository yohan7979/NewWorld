// copyrighted by swonlee ... twiclok@naver.com

#include "CSPickupBase.h"
#include "CSCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"
#include "CSPlayerController.h"

ACSPickupBase::ACSPickupBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CapsuleComponent = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleSize(50.f, 50.f, false);
	CapsuleComponent->SetCollisionObjectType(ECC_WorldDynamic);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(COLLISION_INTERACTIVE, ECR_Overlap);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetGenerateOverlapEvents(true);

	RootComponent = CapsuleComponent;

	StaticMeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetupAttachment(RootComponent);

	SetReplicates(true);

	bCanBePickedup = true;
}

void ACSPickupBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ACSPickupBase::CanInteractWith(class ACSCharacter* Character)
{
	return bCanBePickedup;
}

void ACSPickupBase::OnInteractWith(class ACSCharacter* Character)
{
	PreGiveTo(Character);
	GiveTo(Character);
	PostGiveTo(Character);
}

void ACSPickupBase::PreGiveTo(class ACSCharacter* Character)
{
	if (PickupSound)
	{
		APlayerController* PC = IsValid(Character) ? Character->GetController<APlayerController>(): nullptr;
		if (PC)
		{
			PC->ClientPlaySound(PickupSound);
		}
	}
}

void ACSPickupBase::GiveTo(class ACSCharacter* Character)
{

}

void ACSPickupBase::PostGiveTo(class ACSCharacter* Character)
{
	Destroy();
}