// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeapon_BaseSword.h"
#include <Animation/AnimMontage.h>
#include "NewWorld/Framework/Character/CSCharacter.h"

ACSWeapon_BaseSword::ACSWeapon_BaseSword(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	AttachPoint.GripSocketName = TEXT("sword_use");
	AttachPoint.UnEquipSocketName = TEXT("sword_back");
}

void ACSWeapon_BaseSword::PlayWeaponAnimation(const FWeaponAnim& InWeaponAnim)
{
	Super::PlayWeaponAnimation(InWeaponAnim);

	if (CachedCharacter)
	{
		UAnimMontage* CurrentMotange = CachedCharacter->GetCurrentMontage();
		if (CurrentMotange)
		{
			const float MontageLength = CurrentMotange->GetPlayLength();
			TArray<FAnimNotifyEventReference> NotifyEventReference;
			CurrentMotange->GetAnimNotifiesFromDeltaPositions(0.f, MontageLength, NotifyEventReference);

			for (const FAnimNotifyEventReference& it : NotifyEventReference)
			{
				if (it.GetNotify()->GetNotifyEventName() == TEXT("AnimNotify_CSHit"))
				{
					HitTimes.Add(it.GetNotify()->GetTime() / CurrentMotange->RateScale);
				}				
			}
		}
	}
}
