// Copyleft, Andrey Protsenko. No rights reserved.


#include "Callbacks/NROS_CreateSessionCallbackProxy.h"
#include "Kismet/GameplayStatics.h"
#include "NROS_GameInstance.h"

UNROS_CreateSessionCallbackProxy* UNROS_CreateSessionCallbackProxy::CreateNetRunnerSession(UObject* WorldContextObject, const FNROS_HostSessionParameters& Params)
{
	UNROS_CreateSessionCallbackProxy* OutProxy = NewObject<UNROS_CreateSessionCallbackProxy>(WorldContextObject);
	OutProxy->SessionParams = Params;
	return OutProxy;
}

void UNROS_CreateSessionCallbackProxy::Activate()
{
	UNROS_SessionController* pSessionController = GetSessionController();
	if (pSessionController)
	{
		pSessionController->Public_OnSessionCreated.BindUObject(this, &UNROS_CreateSessionCallbackProxy::OnCompleted);
		pSessionController->CreateSession(SessionParams);
		return;
	}
	OnFailure.Broadcast();
}

void UNROS_CreateSessionCallbackProxy::OnCompleted(bool bWasSuccessful)
{
	UNROS_SessionController* pSessionController = GetSessionController();
	if (pSessionController)
	{
		pSessionController->Public_OnSessionCreated.Unbind();
		
	}

	if (bWasSuccessful)
	{
		OnSuccess.Broadcast();
		return;
	}
	OnFailure.Broadcast();
}

UNROS_DestroySessionCallbackProxy* UNROS_DestroySessionCallbackProxy::DestroyNetRunnerSession(UObject* WorldContextObject)
{
	return NewObject<UNROS_DestroySessionCallbackProxy>(WorldContextObject);

}

void UNROS_DestroySessionCallbackProxy::Activate()
{
	UNROS_SessionController* SessionController = GetSessionController();
	if (!SessionController)
	{
		OnFailure.Broadcast();
		return;
	}

	SessionController->Public_OnSessionDestroyed.BindUObject(this, &UNROS_DestroySessionCallbackProxy::OnCompleted);
	SessionController->DestroyCurrentSession(false);
}

void UNROS_DestroySessionCallbackProxy::OnCompleted(bool bSuccessful)
{
	UNROS_SessionController* SessionController = GetSessionController();
	if (!SessionController)
	{
		OnFailure.Broadcast();
		return;
	}

	SessionController->Public_OnSessionDestroyed.Unbind();
	if (bSuccessful)
	{
		OnSuccess.Broadcast();
		return;
	}
	OnFailure.Broadcast();
}
