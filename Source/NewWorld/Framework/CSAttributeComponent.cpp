// copyrighted by swonlee ... twiclok@naver.com


#include "CSAttributeComponent.h"
#include "CSCharacter.h"

UCSAttributeComponent::UCSAttributeComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHealth = 200.f;

	SetIsReplicatedByDefault(true);
}

void UCSAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACSCharacter>(GetOwner());
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacter->OnTakeAnyDamage.AddDynamic(this, &UCSAttributeComponent::HandleTakeDamage);
	}
}

void UCSAttributeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OwnerCharacter)
	{
		OwnerCharacter->OnTakeAnyDamage.RemoveAll(this);
		OwnerCharacter = nullptr;
	}
}


void UCSAttributeComponent::HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	// @todo : 아군 / 적군 데미지 제한

	// 음수 데미지도 고려
	if (Damage != 0.f)
	{
		Health = FMath::Clamp<float>(Health - Damage, 0.f, MaxHealth);

		UE_LOG(LogTemp, Log, TEXT("HandleTakeDamage, Damage : %f"), Damage);

		OnRep_Health();
	}
}

void UCSAttributeComponent::OnRep_Health()
{
	if (OnHealthChangedEvent.IsBound())
	{
		OnHealthChangedEvent.Broadcast(Health);
	}
}

void UCSAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCSAttributeComponent, Health);
	DOREPLIFETIME(UCSAttributeComponent, CharacterStatus);
}