// copyrighted by swonlee ... twiclok@naver.com


#include "CSAnimNotifyState_Trail.h"
#include "CSCharacter.h"
#include "CSWeapon.h"
#include "Kismet/GameplayStatics.h"

UCSAnimNotifyState_Trail::UCSAnimNotifyState_Trail(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

UParticleSystem* UCSAnimNotifyState_Trail::GetOverridenPSTemplate(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) const
{
	if (MeshComp)
	{
		if (ACSCharacter* OwnerCharacter = Cast<ACSCharacter>(MeshComp->GetOwner()))
		{
			if (OwnerCharacter->Weapon && OwnerCharacter->Weapon->WeaponTrailFX.TrailParticle.IsNull() == false)
			{
				return OwnerCharacter->Weapon->WeaponTrailFX.TrailParticle.LoadSynchronous();
			}
		}
	}

	return nullptr;
}

void UCSAnimNotifyState_Trail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	// play main trail particle
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (MeshComp)
	{
		if (ACSCharacter* OwnerCharacter = Cast<ACSCharacter>(MeshComp->GetOwner()))
		{
			if (ACSWeapon* OwnerWeapon = OwnerCharacter->Weapon)
			{
				if (OwnerWeapon->WeaponTrailFX.SubParticles.Num() > 0)
				{
					SubPSTemplates.Empty();
					for (auto&& it : OwnerCharacter->Weapon->WeaponTrailFX.SubParticles)
					{
						if (false == it.Key.IsNull())
						{
							UParticleSystem* PS = it.Key.LoadSynchronous();
							if (PS)
							{
								SubPSTemplates.Add(PS, it.Value);
							}
						}

					}
				}
				
			}
		}
	}

	// play sub particles
	SpawnParticleSystems(MeshComp, Animation);
}

void UCSAnimNotifyState_Trail::SpawnParticleSystems(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	for (auto&& it: SubPSTemplates)
	{
		UParticleSystem* PS = it.Key;
		if (nullptr == PS)
		{
			continue;
		}

		if (PS->IsLooping())
		{
			continue;
		}

		UGameplayStatics::SpawnEmitterAttached(PS, MeshComp, it.Value, FVector::ZeroVector, FRotator::ZeroRotator, FVector::OneVector);
	}
}
