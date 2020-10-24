// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "NewWorld/Framework/CSModifierProperties.h"
#include "CSModifierPropertiesImpl.generated.h"

/**
 * 
 */
UCLASS(DisplayName = "Recovery")
class NEWWORLD_API UCSModifierProperties_Recovery : public UCSModifierProperties
{
	GENERATED_BODY()

protected:
	UCSModifierProperties_Recovery(const FObjectInitializer& ObjectInitializer);
	virtual void CopyProperties(UCSModifierComponent* TargetComponent) override;

	UPROPERTY(EditDefaultsOnly)
	float RecoveryHealth;
};
