// Copyleft, Andrey Protsenko. No rights reserved.


#include "UI/NRWidget.h"
#include "NRGameInstance.h"
#include "Player/NRPlayerStateBase.h"
#include "Controller/NRPlayerControllerBase.h"

UNRGameInstance* UNRWidget::GetNRGameInstance() const
{
	return GetGameInstance<UNRGameInstance>();
}

ANRPlayerControllerBase* UNRWidget::GetOwningNRPlayerController() const
{
	return GetOwningPlayer<ANRPlayerControllerBase>();
}

ANRPlayerStateBase* UNRWidget::GetOwningNRPlayerState() const
{
	return GetOwningNRPlayerController()->GetPlayerState<ANRPlayerStateBase>();
}
