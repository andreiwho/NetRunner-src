// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Game/NRUsableItemBase.h"
#include "NRPlayerControllerBase.generated.h"

class UNRGameInstance;
class ANRCheckPointBase;
class UNRInGameUIContainer;

UCLASS()
class NETRUNNER_API ANRPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	// This is being used only on server
	UPROPERTY(Transient)
	ANRCheckPointBase* LastCheckpoint;

	// Called from GameMode (server)
	void OnMatchStateSet(FName MatchState);

	// UI
	void UI_UpdateCharacterSpeedModifier(float SpeedModifier);

	// Called on server
	void PickedUpItem(TSubclassOf<ANRUsableItemBase> ItemClass);

	UFUNCTION(Server, Reliable)
	void Server_UseCurrentItem();

	UFUNCTION(BlueprintCallable)
	void OpenOrCloseMenu();

	virtual void Tick(float DeltaSeconds) override;

	void PlayerFinished(APlayerState* PlayerState);

	// Gets called when owning player finishes the map
	UFUNCTION(Client, Reliable)
	void Client_PlayerFinished(int32 PlayerPlace);

	FORCEINLINE bool HasAlreadyFinished() const { return bAlreadyFinished; }

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "On Waiting new Map"))
	void ReceiveWaitingNewMap();

	UFUNCTION(Client,Reliable)
	void Client_ReceiveWaitingNewMap();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UNRGameInstance* GetNRGameInstance() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintNativeEvent, meta=(DisplayName="On Match State Set"))
	void ReceiveOnMatchStateSet(FName MatchState);

	UPROPERTY(Replicated)
	bool bAlreadyFinished = false;

protected:
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UNRInGameUIContainer> InGameUIContainerClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="UI")
	UNRInGameUIContainer* UIContainer;

	UPROPERTY(ReplicatedUsing=OnRep_CurrentMatchState)
	FName CurrentMatchState;

	UFUNCTION()
	void OnRep_CurrentMatchState();

	UPROPERTY(ReplicatedUsing=OnRep_CurrentItem)
	TSubclassOf<ANRUsableItemBase> CurrentItem;

	UFUNCTION()
	void OnRep_CurrentItem();

	void ReceivedPickUpItem();

private:
	void InitInGameUIContainer();

private:
	UFUNCTION(Server, Reliable)
	void Server_RequestTimeDifference(float TimeRequested);

	UFUNCTION(Client, Reliable)
	void Client_ResponseTimeDifference(float ClientTimeRequested, float ServerTimeResponse);

	void WaitingNewMap();

	float ClientServerTimeDifference = 0.0f;
};
