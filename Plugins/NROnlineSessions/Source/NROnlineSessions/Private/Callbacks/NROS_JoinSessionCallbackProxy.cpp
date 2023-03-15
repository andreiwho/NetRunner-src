// Copyleft, Andrey Protsenko. No rights reserved.

#include "Callbacks/NROS_JoinSessionCallbackProxy.h"
#include "NROS_SessionController.h"

UNROS_JoinSessionCallbackProxy* UNROS_JoinSessionCallbackProxy::JoinNetRunnerSession(UObject* WorldContextObject, const FNROS_SessionSearchResult& SessionSearchResult)
{
	UNROS_JoinSessionCallbackProxy* OutProxy = NewObject<UNROS_JoinSessionCallbackProxy>(WorldContextObject);
	OutProxy->SearchResultToJoin = SessionSearchResult;
	return OutProxy;
}

void UNROS_JoinSessionCallbackProxy::Activate()
{
	UNROS_SessionController* pSessionController = GetSessionController();
	if (!pSessionController)
	{
		OnFailure.Broadcast({});
		return;
	}
	
	pSessionController->Public_OnSessionJoined.BindUObject(this, &ThisClass::OnCompleted);
	pSessionController->JoinSession(SearchResultToJoin);
}

void UNROS_JoinSessionCallbackProxy::OnCompleted(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnSuccess.Broadcast(GetSessionController()->GetSessionConnectString(SearchResultToJoin));
	}
	else
	{
		OnFailure.Broadcast({});
	}
}
