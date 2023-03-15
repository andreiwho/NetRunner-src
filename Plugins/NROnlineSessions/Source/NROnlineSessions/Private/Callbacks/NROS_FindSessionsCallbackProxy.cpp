// Copyleft, Andrey Protsenko. No rights reserved.


#include "Callbacks/NROS_FindSessionsCallbackProxy.h"
#include "NROS_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "NROS_SessionController.h"

UNROS_FindSessionsCallbackProxy* UNROS_FindSessionsCallbackProxy::FindNetRunnerSessions(UObject* WorldContextObject)
{
	UNROS_FindSessionsCallbackProxy* OutProxy = NewObject<UNROS_FindSessionsCallbackProxy>(WorldContextObject);
	return OutProxy;
}

void UNROS_FindSessionsCallbackProxy::Activate()
{
	UNROS_SessionController* pSessionController = GetSessionController();
	if (pSessionController)
	{
		pSessionController->Public_OnSessionsFound.BindUObject(this, &UNROS_FindSessionsCallbackProxy::OnCompleted);
		pSessionController->FindSessions();
		return;
	}
	OnFailure.Broadcast({});
}

void UNROS_FindSessionsCallbackProxy::OnCompleted(bool bWasSuccessful)
{
	UNROS_SessionController* pSessionController = GetSessionController();
	if (pSessionController)
	{
		pSessionController->Public_OnSessionsFound.Unbind();
	}

	if (bWasSuccessful)
	{
		SearchResults = pSessionController->GetSessionSearchResults();
		OnSuccess.Broadcast(SearchResults);
		return;
	}
	OnFailure.Broadcast({});
}
