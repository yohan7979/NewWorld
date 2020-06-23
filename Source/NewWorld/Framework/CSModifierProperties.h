// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "CSModifierComponent.h"
#include "CSModifierProperties.generated.h"

/**
 * 
 */
UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class NEWWORLD_API UCSModifierProperties : public UObject
{
	GENERATED_BODY()

protected:
	UCSModifierProperties(const FObjectInitializer& ObjectInitializer);
	virtual void CopyProperties(UCSModifierComponent* TargetComponent) PURE_VIRTUAL(UCSModifierProperties::CopyProperties,)

	UPROPERTY(VisibleDefaultsOnly)
	TSubclassOf<class UCSModifierComponent> ModifierComponentClass;

public:
	virtual UCSModifierComponent* GetNewComponent(AActor* TargetActor);
};
