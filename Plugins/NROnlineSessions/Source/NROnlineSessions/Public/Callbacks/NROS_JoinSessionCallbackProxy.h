// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Callbacks/NROS_OnlineCallbackProxyBase.h"
#include "NROS_PublicTypes.h"
#include "NROS_JoinSessionCallbackProxy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FNROS_JoinSessionCallbackDelegate, FString, ConnectString);

UCLASS()
class NRONLINESESSIONS_API UNROS_JoinSessionCallbackProxy : public UNROS_OnlineCallbackProxyBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UNROS_JoinSessionCallbackProxy* JoinNetRunnerSession(UObject* WorldContextObject, const FNROS_SessionSearchResult& SessionSearchResult);

	UPROPERTY(BlueprintAssignable)
	FNROS_JoinSessionCallbackDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FNROS_JoinSessionCallbackDelegate OnFailure;

	virtual void Activate() override;

	void OnCompleted(bool bWasSuccessful);

private:
	FNROS_SessionSearchResult SearchResultToJoin;
};
