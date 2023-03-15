// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NRPlatformBase.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class ANRCharacterBase;
class USoundCue;

UCLASS()
class NETRUNNER_API ANRPlatformBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ANRPlatformBase();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Platform Base")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Platform Base")
	UBoxComponent* BoxCollision;
	
	UPROPERTY(EditAnywhere, Category = "Platform Base")
	USoundCue* ApplySound;

	virtual void ApplyModifier(ANRCharacterBase* Character);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayApplySound();

	virtual bool ShouldPlayApplySound() const;

private:
	// Executed only on server.
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
