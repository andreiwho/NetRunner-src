// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NRCheckPointBase.generated.h"

class UBoxComponent;

UCLASS()
class NETRUNNER_API ANRCheckPointBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANRCheckPointBase();

	virtual void BeginPlay() override;

	virtual FTransform GetRestartTransform() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Trigger")
	UBoxComponent* BoxCollision;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
