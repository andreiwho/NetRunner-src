// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Game/NRUsableItemBase.h"
#include "NRTriggeredItem.generated.h"

class USphereComponent;
class ANRCharacterBase;

UCLASS()
class NETRUNNER_API ANRTriggeredItem : public ANRUsableItemBase
{
	GENERATED_BODY()
	
public:
	ANRTriggeredItem();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category="Collision")
	USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, Category="Item")
	bool bDestroyOnTrigger = true;

	UPROPERTY(EditAnywhere, Category="Item")
	bool bAffectsOwner = false;

	virtual void TriggeredByPawn(ANRCharacterBase* Character);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
