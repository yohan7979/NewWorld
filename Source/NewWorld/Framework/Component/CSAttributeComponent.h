// copyrighted by swonlee ... twiclok@naver.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NewWorld/Framework/CSTypes.h"
#include "CSAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedEvent, float, CurrentHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NEWWORLD_API UCSAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCSAttributeComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	virtual void OnRep_Health();

public:	
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health;

	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxHealth;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedEvent OnHealthChangedEvent;

	UPROPERTY(Transient)
	class ACSCharacter* OwnerCharacter;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FCharacterStatus CharacterStatus;
};
