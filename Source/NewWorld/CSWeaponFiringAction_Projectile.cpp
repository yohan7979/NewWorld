// Fill out your copyright notice in the Description page of Project Settings.


#include "CSWeaponFiringAction_Projectile.h"
#include "Kismet/GameplayStatics.h"

UCSWeaponFiringAction_Projectile::UCSWeaponFiringAction_Projectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UCSWeaponFiringAction_Projectile::FireShot()
{
	ACSWeapon* OwnerWeapon = GetOwnerWeapon();
	if (IsValid(OwnerWeapon) && OwnerWeapon->HasAuthority())
	{
		const FVector& Origin = OwnerWeapon->GetMuzzleLocation();
		const FVector& AimDir = OwnerWeapon->GetAdjustedAim();
		
		const FVector& StartTrace = OwnerWeapon->GetCameraStartLocation(AimDir);
		const FVector& EndTrace = StartTrace + AimDir * 1000.f;
		
		FVector AdjustedDir = AimDir;

		FHitResult Hit(ForceInit);
		if (OwnerWeapon->WeaponTrace(StartTrace, EndTrace, Hit))
		{
			AdjustedDir = (Hit.ImpactPoint - Origin).GetSafeNormal();
		}

		SpawnProjectile(OwnerWeapon, Origin, AdjustedDir);
	}
}

void UCSWeaponFiringAction_Projectile::SpawnProjectile(class ACSWeapon* OwnerWeapon, const FVector& Origin, const FVector& ShotDirection)
{
	FTransform SpawnTM(FRotator::ZeroRotator, Origin);

	ACSProjectile* Projectile = Cast<ACSProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileWeaponConfig.ProjectileClass, SpawnTM, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, OwnerWeapon->Instigator));
	if (Projectile)
	{
		Projectile->Instigator = OwnerWeapon->Instigator;
		Projectile->SetOwner(OwnerWeapon);
		Projectile->Init(ShotDirection, ProjectileWeaponConfig);

		DrawDebugLine(GetWorld(), Origin, Origin + ShotDirection * 1000.f, FColor::Red, true, 3.f);

		UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);
	}
}
