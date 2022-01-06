// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSModifierComponent.generated.h"

class ACSCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWWORLD_API UCSModifierComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UCSModifierComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual bool ShouldActivate() const override;
	virtual void InternalApplyTo(AActor* TargetActor) {}
	virtual void InternalRestoreFrom(AActor* TargetActor) {}

public:
	virtual bool ApplyTo(AActor* TargetActor);
	virtual void RestoreFrom(AActor* TargetActor);

protected:
	UPROPERTY(Transient)
	ACSCharacter* CachedCharacter;

	bool bIsApplied;
};
