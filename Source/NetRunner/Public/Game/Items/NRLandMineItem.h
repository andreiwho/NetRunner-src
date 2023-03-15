// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Game/Items/NRTriggeredItem.h"
#include "NRLandMineItem.generated.h"

class UParticleSystem;
class UStaticMeshComponent;
class USoundCue;

UCLASS()
class NETRUNNER_API ANRLandMineItem : public ANRTriggeredItem
{
	GENERATED_BODY()
public:
	ANRLandMineItem();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category="Item")
	UParticleSystem* ExplosionParticleSystem;

	UPROPERTY(EditAnywhere, Category="Item")
	USoundCue* PlacementSound;

	UPROPERTY(EditAnywhere, Category="Item")
	USoundCue* ExplosionSound;

	UPROPERTY(EditAnywhere, Category="Item")
	float LaunchForce = 100.0f;
	
	UPROPERTY(EditAnywhere, Category="Item")
	float DestroyAfter = 10.0f;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Explode();

	virtual void LifeSpanExpired() override;

protected:
	virtual void TriggeredByPawn(ANRCharacterBase* Character) override;

private:
	FVector CalculateLaunchVelocity(const FVector& LaunchTargetLocation) const;
};
