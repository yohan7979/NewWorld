// copyrighted by swonlee ... twiclok@naver.com


#include "CSModifierPropertiesImpl.h"
#include "CSModifierComponent_Recovery.h"

UCSModifierProperties_Recovery::UCSModifierProperties_Recovery(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ModifierComponentClass = UCSModifierComponent_Recovery::StaticClass();
}

void UCSModifierProperties_Recovery::CopyProperties(UCSModifierComponent* TargetComponent)
{
	UCSModifierComponent_Recovery* NewComponent = Cast<UCSModifierComponent_Recovery>(TargetComponent);
	if (NewComponent)
	{
		NewComponent->RecoveryHealth = RecoveryHealth;
	}
}
