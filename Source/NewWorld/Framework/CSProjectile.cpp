// Fill out your copyright notice in the Description page of Project Settings.


#include "CSProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "CSWeapon.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "CSGameplayStatics.h"
#include "CSCharacter.h"

ACSProjectile::ACSProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SphereComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	SphereComponent->SetSphereRadius(50.f);
	SphereComponent->SetCollisionProfileName(TEXT("Projectile"));
	SphereComponent->SetGenerateOverlapEvents(true);
	RootComponent = SphereComponent;

	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	ParticleComponent = ObjectInitializer.CreateDefaultSubobject<UParticleSystemComponent>(this, TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(RootComponent);

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void ACSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (SphereComponent)
	{
		SphereComponent->OnComponentHit.AddDynamic(this, &ACSProjectile::OnHit);
		SphereComponent->MoveIgnoreActors.Add(Instigator);
	}

	PlayParticleEffect(ParticleSet.SpawnParticle);

	const float LifeSpan = FMath::Min(ProjectileConfig.ProjectileLife, 15.f);
	SetLifeSpan(LifeSpan);
}

void ACSProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (SphereComponent)
	{
		SphereComponent->OnComponentHit.RemoveAll(this);
	}

	SpawnedParticles.Empty();
}

void ACSProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ParticleComponent)
	{
		UParticleSystem* MovableParticle = UCSGameplayStatics::GetLoadedObject<UParticleSystem>(ParticleSet.MovableParticle);
		ParticleComponent->SetTemplate(MovableParticle);
		ParticleComponent->Activate();
	}
}

void ACSProjectile::Init(const FVector& Direction, const FProjectileWeaponConfig& InProjectileConfig)
{
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
	ProjectileConfig = InProjectileConfig;
}

void ACSProjectile::Explode()
{
	if (IsValid(ParticleComponent))
	{
		ParticleComponent->Deactivate();
		ParticleComponent->MarkPendingKill();
		ParticleComponent = nullptr;
	}

	PlayParticleEffect(ParticleSet.HitParticle);

	SetLifeSpan(0.5f);

	bExploded = true;
}

void ACSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsPendingKill() && !bExploded)
	{
		Explode();

		if (Role == ROLE_Authority)
		{
			if (IsValid(OtherActor))
			{
				FDamageEvent DamageEvent(ProjectileConfig.DamageType);
				OtherActor->TakeDamage(ProjectileConfig.ExplosionDamage, DamageEvent, GetInstigatorController(), GetInstigator());
			}
		}
	}
}

void ACSProjectile::PlayParticleEffect(TSoftObjectPtr<UParticleSystem>& SoftObjectPtr, bool bAttachToSocket)
{
	if (IsNetMode(NM_DedicatedServer))
	{
		return;
	}

	UParticleSystem* TargetParticle = UCSGameplayStatics::GetLoadedObject<UParticleSystem>(SoftObjectPtr);
	if (IsValid(TargetParticle))
	{
		ACSWeapon* OwnerWeapon = Cast<ACSWeapon>(GetOwner());
		if (bAttachToSocket && IsValid(OwnerWeapon))
		{
			UGameplayStatics::SpawnEmitterAttached(TargetParticle, OwnerWeapon->GetMeshComponent(), OwnerWeapon->GetMuzzleSocketName());
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TargetParticle, GetTransform());
		}
		
		SpawnedParticles.Add(TargetParticle);
	}
}
