// Copyleft, Andrey Protsenko. No rights reserved.

#include "Game/Items/NRLandMineItem.h"
#include "Components/SphereComponent.h"
#include "Character/NRCharacterBase.h"
#include "Sound/SoundCue.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

ANRLandMineItem::ANRLandMineItem()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (StaticMesh)
	{
		StaticMesh->SetupAttachment(GetRootComponent());
		StaticMesh->SetSimulatePhysics(true);
		StaticMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}

	if (SphereCollision)
	{
		SphereCollision->SetSphereRadius(100.0f);
	}
	bDestroyOnTrigger = true;
}

void ANRLandMineItem::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* pWorld = GetWorld())
	{
		if (PlacementSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(pWorld, PlacementSound, GetActorLocation(), FRotator::ZeroRotator, 1.0f, FMath::RandRange(0.8f, 1.2f));
		}
		SetLifeSpan(DestroyAfter);
	}
}

void ANRLandMineItem::Multicast_Explode_Implementation()
{
	if (UWorld* pWorld = GetWorld())
	{
		if (ExplosionParticleSystem)
		{
			UGameplayStatics::SpawnEmitterAtLocation(pWorld, ExplosionParticleSystem, StaticMesh->GetComponentTransform());
		}

		if (ExplosionSound)
		{
			UGameplayStatics::SpawnSoundAtLocation(pWorld, ExplosionSound, StaticMesh->GetComponentLocation(), StaticMesh->GetComponentRotation());
		}
	}
}

void ANRLandMineItem::LifeSpanExpired()
{
	Multicast_Explode();

	Super::LifeSpanExpired();
}

void ANRLandMineItem::TriggeredByPawn(ANRCharacterBase* Character)
{
	Super::TriggeredByPawn(Character);

	TArray<FHitResult> Hits;

	if (UWorld* pWorld = GetWorld())
	{
		bool bIsHit = pWorld->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_Pawn, SphereCollision->GetCollisionShape());
		if (!bIsHit)
		{
			return; // Which is not really the case, because this is only triggered when a character enters the forcefield
		}

		for (const FHitResult& HitResult : Hits)
		{
			if (!HitResult.Actor.IsValid())
			{
				continue;
			}

			// Don't affect the owner actor (maybe a subject to reconsider)
			if (HitResult.Actor == GetOwner() && !bAffectsOwner)
			{
				continue;
			}

			const FVector LaunchDirection = CalculateLaunchVelocity(HitResult.Actor->GetActorLocation());
			if (ANRCharacterBase* pCharacter = Cast<ANRCharacterBase>(HitResult.Actor.Get()))
			{
				pCharacter->LaunchCharacter(LaunchDirection, true, true);
			}
		}

		Multicast_Explode();
	}
}

FVector ANRLandMineItem::CalculateLaunchVelocity(const FVector& LaunchTargetLocation) const
{
	FVector OutDirection = (LaunchTargetLocation - GetActorLocation()).GetSafeNormal();
	OutDirection.Z = 1.0f;
	return OutDirection * LaunchForce;
}
