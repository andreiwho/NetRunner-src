// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Game/NRPlatformBase.h"
#include "NRJumpPlatform.generated.h"

class UArrowComponent;

UCLASS()
class NETRUNNER_API ANRJumpPlatform : public ANRPlatformBase
{
	GENERATED_BODY()

public:
	ANRJumpPlatform();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchVelocity = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchAngle = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideXYVelocity = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverrideZVelocity = true;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetLaunchDirectionVector() const;

protected:
	virtual void ApplyModifier(ANRCharacterBase* Character) override;

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UArrowComponent* LaunchDirectionArrow;
};
