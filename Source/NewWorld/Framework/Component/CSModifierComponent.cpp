// copyrighted by swonlee ... twiclok@naver.com


#include "CSModifierComponent.h"
#include "NewWorld/Framework/Character/CSCharacter.h"

UCSModifierComponent::UCSModifierComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCSModifierComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedCharacter = Cast<ACSCharacter>(GetOwner());
}

void UCSModifierComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool UCSModifierComponent::ShouldActivate() const
{
	return Super::ShouldActivate();
}

bool UCSModifierComponent::ApplyTo(AActor* TargetActor)
{
	if (TargetActor)
	{
		TargetActor->AddOwnedComponent(this);
		RegisterComponent();
		InternalApplyTo(TargetActor);

		bIsApplied = true;
		return true;
	}

	return false;
}

void UCSModifierComponent::RestoreFrom(AActor* TargetActor)
{
	if (bIsApplied)
	{
		if (IsValid(TargetActor))
		{
			TargetActor->RemoveOwnedComponent(this);

			InternalRestoreFrom(TargetActor);
		}

		UnregisterComponent();
		bIsApplied = false;
	}
}
