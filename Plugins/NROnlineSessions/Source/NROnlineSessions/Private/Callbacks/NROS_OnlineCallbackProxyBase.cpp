// Copyleft, Andrey Protsenko. No rights reserved.


#include "Callbacks/NROS_OnlineCallbackProxyBase.h"
#include "Kismet/GameplayStatics.h"
#include "NROS_GameInstance.h"
#include "NROS_SessionController.h"

UNROS_SessionController* UNROS_OnlineCallbackProxyBase::GetSessionController() const
{
	UWorld* pWorld = GetWorld();
	if (!pWorld)
	{
		return nullptr;
	}

	UNROS_GameInstance* pGameInstance = Cast<UNROS_GameInstance>(UGameplayStatics::GetGameInstance(pWorld));
	if (pGameInstance)
	{
		UNROS_SessionController* pSessionController = pGameInstance->GetSessionController();
		if (pSessionController)
		{
			return pSessionController;
		}
	}
	return nullptr;
}
