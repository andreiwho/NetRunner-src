// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Game/NRPlatformBase.h"
#include "NRItemPlatform.generated.h"

class ANRUsableItemBase;
class UParticleSystem;

UCLASS()
class NETRUNNER_API ANRItemPlatform : public ANRPlatformBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	virtual void ApplyModifier(ANRCharacterBase* Character) override;

	void SpawnRandomItem();

	void FreePlatform();

	virtual bool ShouldPlayApplySound() const override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DisplayParticles();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DestroyParticles();

	UPROPERTY()
	TSubclassOf<ANRUsableItemBase> CurrentSpawnedItem;

	UPROPERTY(EditAnywhere, Category="Spawn")
	TArray<TSubclassOf<ANRUsableItemBase>> AllowedItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawn")
	float RespawnRateSeconds = 10.0f;

	UPROPERTY(EditAnywhere, Category="Spawn")
	UParticleSystem* ParticleSystemToSpawn;

	UPROPERTY()
	UParticleSystemComponent* SpawnedParticleSystem;

	FTimerHandle RespawnTimer;
};
