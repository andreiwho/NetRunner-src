// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/OnlineBlueprintCallProxyBase.h"
#include "NROS_OnlineCallbackProxyBase.generated.h"

class UNROS_SessionController;

UCLASS()
class NRONLINESESSIONS_API UNROS_OnlineCallbackProxyBase : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()
	
protected:
	UNROS_SessionController* GetSessionController() const;

};
