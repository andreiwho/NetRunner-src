// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Game/NRPlatformBase.h"
#include "NRFinishPlatform.generated.h"

class UArrowComponent;

UCLASS()
class NETRUNNER_API ANRFinishPlatform : public ANRPlatformBase
{
	GENERATED_BODY()
	
public:
	ANRFinishPlatform();

	virtual void ApplyModifier(ANRCharacterBase* Character) override;
	
	UPROPERTY()
	UArrowComponent* Arrow;
};
