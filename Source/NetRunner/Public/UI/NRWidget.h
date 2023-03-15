// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NRWidget.generated.h"

class UNRGameInstance;
class ANR_HUDBase;
class ANRPlayerControllerBase;
class ANRPlayerStateBase;

UCLASS()
class NETRUNNER_API UNRWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UNRGameInstance* GetNRGameInstance() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ANRPlayerControllerBase* GetOwningNRPlayerController() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ANRPlayerStateBase* GetOwningNRPlayerState() const;
};
