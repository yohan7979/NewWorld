// copyrighted by swonlee ... twiclok@naver.com


#include "CSModifierProperties.h"



UCSModifierProperties::UCSModifierProperties(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

UCSModifierComponent* UCSModifierProperties::GetNewComponent(AActor* TargetActor)
{
	UCSModifierComponent* NewComponent = NewObject<UCSModifierComponent>(TargetActor, ModifierComponentClass);
	if (NewComponent)
	{
		CopyProperties(NewComponent);
	}

	return NewComponent;
}
