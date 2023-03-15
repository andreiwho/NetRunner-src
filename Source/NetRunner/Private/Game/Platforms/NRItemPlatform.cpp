// Copyleft, Andrey Protsenko. No rights reserved.


#include "Game/Platforms/NRItemPlatform.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Controller/NRPlayerControllerBase.h"
#include "Character/NRCharacterBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogItemPlatform, Log, All);

void ANRItemPlatform::BeginPlay()
{
	Super::BeginPlay();

	SpawnRandomItem();
}

void ANRItemPlatform::ApplyModifier(ANRCharacterBase* Character)
{
	Super::ApplyModifier(Character);
	if (!CurrentSpawnedItem)
	{
		return;
	}

	if (!Character)
	{
		FreePlatform();
		return;
	}

	if (ANRPlayerControllerBase* pPlayerController = Character->GetNRPlayerController())
	{
		pPlayerController->PickedUpItem(CurrentSpawnedItem);
	}

	FreePlatform();
}

void ANRItemPlatform::SpawnRandomItem()
{
	if (UWorld* pWorld = GetWorld())
	{
		pWorld->GetTimerManager().ClearTimer(RespawnTimer);

		if (AllowedItems.Num() <= 0)
		{
			UE_LOG(LogItemPlatform, Warning, TEXT("SpawnRandomItem was called, but no allowed items were specified"));
			return;
		}

		CurrentSpawnedItem = AllowedItems[FMath::RandRange(0, AllowedItems.Num() - 1)];
		if (ParticleSystemToSpawn)
		{
			Multicast_DisplayParticles();
		}
	}
}

void ANRItemPlatform::FreePlatform()
{
	Multicast_DestroyParticles();

	if (UWorld* pWorld = GetWorld())
	{
		pWorld->GetTimerManager().SetTimer(RespawnTimer, this, &ANRItemPlatform::SpawnRandomItem, RespawnRateSeconds);
	}

	CurrentSpawnedItem = nullptr;
}

bool ANRItemPlatform::ShouldPlayApplySound() const
{
	return !!CurrentSpawnedItem;
}

void ANRItemPlatform::Multicast_DisplayParticles_Implementation()
{
	if (ParticleSystemToSpawn)
	{
		SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystemToSpawn, GetTransform(), false);
	}
}

void ANRItemPlatform::Multicast_DestroyParticles_Implementation()
{
	if (SpawnedParticleSystem)
	{
		SpawnedParticleSystem->DestroyComponent();
	}
}
