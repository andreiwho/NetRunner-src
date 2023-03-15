// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Callbacks/NROS_OnlineCallbackProxyBase.h"
#include "NROS_SessionController.h"
#include "NROS_CreateSessionCallbackProxy.generated.h"

UCLASS()
class NRONLINESESSIONS_API UNROS_CreateSessionCallbackProxy : public UNROS_OnlineCallbackProxyBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UNROS_CreateSessionCallbackProxy* CreateNetRunnerSession(UObject* WorldContextObject, const FNROS_HostSessionParameters& Params);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

private:
	FNROS_HostSessionParameters SessionParams;
	void OnCompleted(bool bSuccessful);
};

UCLASS()
class NRONLINESESSIONS_API UNROS_DestroySessionCallbackProxy : public UNROS_OnlineCallbackProxyBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static UNROS_DestroySessionCallbackProxy* DestroyNetRunnerSession(UObject* WorldContextObject);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FEmptyOnlineDelegate OnFailure;

private:
	FNROS_HostSessionParameters SessionParams;
	void OnCompleted(bool bSuccessful);
};