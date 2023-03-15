// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/NRWidget.h"
#include "NRInGameUIContainer.generated.h"

class ANRPlayerControllerBase;
class UNRSpeedDisplayWidget;
class UNRInGameMenuWidget;
class UNRPlaceWidget;
class UNRCurrentItemWidget;

/**
 * NRInGameUIContainer - class that manages InGame HUD. Created basically to avoid additional indirection when using AHUD and its.
 * Should only exist on clients and never on server
 */

UCLASS()
class NETRUNNER_API UNRInGameUIContainer : public UNRWidget
{
	GENERATED_BODY()

public:
	virtual void OnMatchStateSet(FName NewState);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="Match State Set"))
	void ReceiveOnMatchStateSet(FName State);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(DisplayName="Open Menu"))
	void ReceiveOnOpenMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(DisplayName="Close Menu"))
	void ReceiveOnCloseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(DisplayName="Update Lap Info"))
	void ReceiveUpdateLapInfo(int32 PlayerPlace, int32 TotalPlayers, int32 Lap);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="Player Finished"))
	void ReceivePlayerFinished(int32 PlayerPlace);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="Waiting for New Map"))
	void ReceiveWaitingForNewMap();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="Item Picked Up"))
	void ReceiveItemPickedUp(TSubclassOf<ANRUsableItemBase> ItemClass);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, meta=(DisplayName="Item Removed"))
	void ReceiveItemRemoved();

	UFUNCTION(BlueprintCallable, BLueprintPure)
	FORCEINLINE bool IsMenuOpened() const { return bIsMenuOpened; }

public:
	// States
	UFUNCTION(BlueprintNativeEvent, Category="Player")
	void UpdateSpeedModifier(float Modifier);

	UPROPERTY(meta=(BindWidget))
	UNRSpeedDisplayWidget* SpeedDisplay;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UNRInGameMenuWidget* EscapeMenu;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UNRPlaceWidget* LapDisplay;
	
private:
	bool bIsMenuOpened = false;
};
