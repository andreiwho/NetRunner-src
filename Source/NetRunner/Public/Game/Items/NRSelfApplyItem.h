// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Game/NRUsableItemBase.h"
#include "NRSelfApplyItem.generated.h"

class ANRPlayerControllerBase;

UCLASS()
class NETRUNNER_API ANRSelfApplyItem : public ANRUsableItemBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Use Item"))
	void ReceiveUseItem(ANRPlayerControllerBase* Player);

	virtual void UseItem(ANRPlayerControllerBase* Player);
};
