// copyrighted by swonlee ... twiclok@naver.com


#include "CSWeaponFiringAction_Melee.h"
#include "CSCharacter.h"
#include "CSWeapon_BaseSword.h"

UCSWeaponFiringAction_Melee::UCSWeaponFiringAction_Melee(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWeaponFiringAction_Melee::StopFire()
{
	Super::StopFire();

	if (ACSWeapon_BaseSword* OwnerWeapon = GetOwnerWeapon<ACSWeapon_BaseSword>())
	{
		OwnerWeapon->HitTimes.Empty();
	}
}

void UCSWeaponFiringAction_Melee::FireShot()
{
	ElapsedFireTime = 0.f;


	Super::FireShot();
}

void UCSWeaponFiringAction_Melee::Tick(float fDeltaTime)
{
	if (ACSWeapon_BaseSword* OwnerWeapon = GetOwnerWeapon<ACSWeapon_BaseSword>())
	{
		ElapsedFireTime += fDeltaTime;

		int32 HitIndex = OwnerWeapon->HitTimes.IndexOfByPredicate([this](float HitTime)
		{
			return ElapsedFireTime >= HitTime;
		});

		if (HitIndex != INDEX_NONE)
		{
			InternalFireShot();
			OwnerWeapon->HitTimes.RemoveAt(HitIndex);
		}
	}

	Super::Tick(fDeltaTime);
}

void UCSWeaponFiringAction_Melee::InternalFireShot()
{
	ACSWeapon_BaseSword* OwnerWeapon = GetOwnerWeapon<ACSWeapon_BaseSword>();
	if (IsValid(OwnerWeapon) && OwnerWeapon->HasAuthority())
	{
		ACSCharacter* OwnerCharacter = OwnerWeapon->GetOwnerCharacter();
		if (IsValid(OwnerCharacter))
		{
			const FMeleeWeaponConfig& WeaponConfig = OwnerWeapon->MeleeWeaponConfig;
			const FVector& Position = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 100.f;

			TArray<FOverlapResult> OverlapResults;
			if (OwnerWeapon->WeaponTrace(Position, FQuat::Identity, FCollisionShape::MakeSphere(WeaponConfig.WeaponRange), OverlapResults))
			{
				TArray<AActor*> HitActors;
				for (const FOverlapResult& Result : OverlapResults)
				{
					if (AActor* TargetActor = Result.GetActor())
					{
						HitActors.AddUnique(TargetActor);
					}					
				}

				for (AActor* HitActor : HitActors)
				{
					if (IsValid(HitActor))
					{
						FDamageEvent DamageEvent(WeaponConfig.DamageType);						
						HitActor->TakeDamage(WeaponConfig.HitDamage, DamageEvent, OwnerCharacter->GetController(), OwnerCharacter);
					}
				}
			}
		}
	}
}
