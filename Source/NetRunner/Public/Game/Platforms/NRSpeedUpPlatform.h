// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Game/NRPlatformBase.h"
#include "NRSpeedUpPlatform.generated.h"

UCLASS()
class NETRUNNER_API ANRSpeedUpPlatform : public ANRPlatformBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpeedUp")
	float SpeedUpModifier = 1.5f;

	virtual void ApplyModifier(ANRCharacterBase* Character) override;
};
