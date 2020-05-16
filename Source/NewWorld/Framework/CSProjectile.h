// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSTypes.h"
#include "CSProjectile.generated.h"

USTRUCT()
struct FProjectileWeaponConfig
{
	GENERATED_USTRUCT_BODY()

	/** projectile class */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ACSProjectile> ProjectileClass;

	/** life time */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	float ProjectileLife;

	/** damage at impact point */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	int32 ExplosionDamage;

	/** radius of damage */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	float ExplosionRadius;

	/** type of damage */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
	TSubclassOf<UDamageType> DamageType;

	/** defaults */
	FProjectileWeaponConfig()
	{
		ProjectileClass = NULL;
		ProjectileLife = 10.0f;
		ExplosionDamage = 100;
		ExplosionRadius = 300.0f;
		DamageType = UDamageType::StaticClass();
	}
};

UCLASS()
class NEWWORLD_API ACSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSProjectile(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void PostInitializeComponents() override;

	virtual void Init(const FVector& Direction, const FProjectileWeaponConfig& InProjectileConfig);
	virtual void Explode();
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void PlayParticleEffect(TSoftObjectPtr<UParticleSystem>& SoftObjectPtr, bool bAttachToSocket = false);

protected:
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* ParticleComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Particles")
	FParticleComponentSet ParticleSet;

	UPROPERTY(Transient)
	FProjectileWeaponConfig ProjectileConfig;

	UPROPERTY(Transient)
	TArray<class UParticleSystem*> SpawnedParticles;

	bool bExploded;
};
