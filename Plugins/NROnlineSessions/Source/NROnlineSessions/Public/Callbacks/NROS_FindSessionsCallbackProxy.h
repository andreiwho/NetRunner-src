// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Callbacks/NROS_OnlineCallbackProxyBase.h"
#include "OnlineSessionSettings.h"
#include "NROS_PublicTypes.h"
#include "NROS_FindSessionsCallbackProxy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNROS_FindSessionCallbackDelegate, const TArray<FNROS_SessionSearchResult>&, Results);

UCLASS()
class NRONLINESESSIONS_API UNROS_FindSessionsCallbackProxy : public UNROS_OnlineCallbackProxyBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UNROS_FindSessionsCallbackProxy* FindNetRunnerSessions(UObject* WorldContextObject);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FNROS_FindSessionCallbackDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FNROS_FindSessionCallbackDelegate OnFailure;

	UPROPERTY(BlueprintReadOnly)
	TArray<FNROS_SessionSearchResult> SearchResults;

private:
	void OnCompleted(bool bSuccessful);
};
