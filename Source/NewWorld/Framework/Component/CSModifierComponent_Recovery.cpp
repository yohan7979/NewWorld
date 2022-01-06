// copyrighted by swonlee ... twiclok@naver.com


#include "CSModifierComponent_Recovery.h"
#include "NewWorld/Framework/Character/CSCharacter.h"

UCSModifierComponent_Recovery::UCSModifierComponent_Recovery(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSModifierComponent_Recovery::BeginPlay()
{
	Super::BeginPlay();
}

void UCSModifierComponent_Recovery::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UCSModifierComponent_Recovery::InternalApplyTo(AActor* TargetActor)
{
	if (ACSCharacter* TargetCharacter = Cast<ACSCharacter>(TargetActor))
	{
		if (GetOwnerRole() == ROLE_Authority)
		{
			FDamageEvent DamageEvent;
			TargetCharacter->TakeDamage(RecoveryHealth * -1, DamageEvent, TargetCharacter->GetController(), nullptr);
		}
		
	}
}
