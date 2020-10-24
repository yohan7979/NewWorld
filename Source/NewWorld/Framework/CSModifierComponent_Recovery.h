// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "NewWorld/Framework/CSModifierComponent.h"
#include "CSModifierComponent_Recovery.generated.h"

/**
 * 
 */
UCLASS()
class NEWWORLD_API UCSModifierComponent_Recovery : public UCSModifierComponent
{
	GENERATED_BODY()
	
protected:
	UCSModifierComponent_Recovery(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void InternalApplyTo(AActor* TargetActor) override;
	
public:
	UPROPERTY(Transient)
	float RecoveryHealth;
};
