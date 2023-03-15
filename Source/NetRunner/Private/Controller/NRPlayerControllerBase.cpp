// Copyleft, Andrey Protsenko. No rights reserved.

#include "Controller/NRPlayerControllerBase.h"
#include "NRGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UI/NRInGameUIContainer.h"
#include "Net/UnrealNetwork.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Functions/NRBlueprintFunctionLibrary.h"
#include "Game/NRUsableItemBase.h"
#include "Game/Items/NRTriggeredItem.h"
#include "Character/NRCharacterBase.h"
#include "Player/NRPlayerStateBase.h"
#include "GameFramework/GameState.h"
#include "UObject/NameTypes.h"
#include "GameFramework/GameMode.h"
#include "Game/Items/NRSelfApplyItem.h"

DEFINE_LOG_CATEGORY_STATIC(LogNRPlayerController, Log, All);

void ANRPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
	if (IsLocalPlayerController())
	{
		if (auto pWorld = GetWorld())
		{
			// Synchronize timing between client and server
			Server_RequestTimeDifference(pWorld->GetTimeSeconds());
		}
		InitInGameUIContainer();
	}

	SetInputMode(FInputModeGameOnly{});
	bShowMouseCursor = false;
}

UNRGameInstance* ANRPlayerControllerBase::GetNRGameInstance() const
{
	return GetGameInstance<UNRGameInstance>();
}

void ANRPlayerControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ANRPlayerControllerBase, CurrentMatchState, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ANRPlayerControllerBase, CurrentItem, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ANRPlayerControllerBase, bAlreadyFinished, COND_OwnerOnly);
}

#pragma region MatchState

void ANRPlayerControllerBase::OnMatchStateSet(FName MatchState)
{
	CurrentMatchState = MatchState;
	ReceiveOnMatchStateSet(CurrentMatchState);
}

void ANRPlayerControllerBase::ReceiveOnMatchStateSet_Implementation(FName InMatchState)
{
	// This will not be valid on server
	if (UIContainer)
	{
		UIContainer->OnMatchStateSet(CurrentMatchState);
	}

	if (InMatchState == MatchState::InProgress)
	{
		bAlreadyFinished = false;
	}
}

void ANRPlayerControllerBase::ReceiveWaitingNewMap_Implementation()
{
	WaitingNewMap();
	Client_ReceiveWaitingNewMap();
}

void ANRPlayerControllerBase::Client_ReceiveWaitingNewMap_Implementation()
{
	WaitingNewMap();

}

void ANRPlayerControllerBase::PlayerFinished(APlayerState* InPlayerState)
{
	ANRPlayerStateBase* pPlayerState = Cast<ANRPlayerStateBase>(InPlayerState);
	if (pPlayerState && pPlayerState == this->PlayerState && !bAlreadyFinished)
	{
		bAlreadyFinished = true;
		Client_PlayerFinished(pPlayerState->PlayerPlace);
	}
}

void ANRPlayerControllerBase::Client_PlayerFinished_Implementation(int32 PlayerPlace)
{
	if (UIContainer)
	{
		UIContainer->ReceivePlayerFinished(PlayerPlace);
	}
}

void ANRPlayerControllerBase::OnRep_CurrentMatchState()
{
	ReceiveOnMatchStateSet(CurrentMatchState);
}

#pragma endregion

#pragma region UI

void ANRPlayerControllerBase::InitInGameUIContainer()
{
	UIContainer = CreateWidget<UNRInGameUIContainer>(this, InGameUIContainerClass);
	check(UIContainer);
	UIContainer->AddToViewport();
}

void ANRPlayerControllerBase::UI_UpdateCharacterSpeedModifier(float SpeedModifier)
{
	if (UIContainer)
	{
		UIContainer->UpdateSpeedModifier(SpeedModifier);
	}
}

void ANRPlayerControllerBase::WaitingNewMap()
{
	if (UIContainer)
	{
		UIContainer->ReceiveWaitingForNewMap();
	}
}

#pragma endregion

#pragma region Sync

void ANRPlayerControllerBase::Server_RequestTimeDifference_Implementation(float TimeRequested)
{
	if (UWorld* pWorld = GetWorld())
	{
		Client_ResponseTimeDifference(TimeRequested, pWorld->GetTimeSeconds());
	}
}

void ANRPlayerControllerBase::Client_ResponseTimeDifference_Implementation(float ClientTimeRequested, float ServerTimeResponse)
{
	if (UWorld* pWorld = GetWorld())
	{
		const float ResponseTimeSeconds = pWorld->GetTimeSeconds();
		ClientServerTimeDifference = ResponseTimeSeconds - ClientTimeRequested;
		UE_LOG(LogNRPlayerController, Display, TEXT("Client server time difference: %f seconds"), ClientServerTimeDifference);
	}
}

#pragma endregion

#pragma region INVENTORY

void ANRPlayerControllerBase::PickedUpItem(TSubclassOf<ANRUsableItemBase> ItemClass)
{
	CurrentItem = ItemClass;
	ReceivedPickUpItem();
}

void ANRPlayerControllerBase::Server_UseCurrentItem_Implementation()
{
	UWorld* pWorld = GetWorld();
	if (!pWorld || !CurrentItem)
	{
		return;
	}

	bool bUsed = false;
	if (CurrentItem->IsChildOf<ANRTriggeredItem>())
	{
		// Get the pawn capsule
		if (ANRCharacterBase* pCharacter = Cast<ANRCharacterBase>(GetPawn()))
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = GetPawn();
			pWorld->SpawnActor<ANRTriggeredItem>(CurrentItem, pCharacter->GetActorLocation(), FRotator::ZeroRotator, SpawnParams);
			bUsed = true;
		}
	}
	if (CurrentItem->IsChildOf<ANRSelfApplyItem>())
	{
		if (ANRSelfApplyItem* pItem = CurrentItem->GetDefaultObject<ANRSelfApplyItem>())
		{
			pItem->UseItem(this);
			bUsed = true;
		}
	}

	if (bUsed)
	{
		UNRBlueprintFunctionLibrary::NR_Log(GetWorld(), ENRPrintLogVerbosity::Display, true, "NRPlayerController", "Removing item from inventory");
		CurrentItem = nullptr;
		ReceivedPickUpItem();
	}
}

void ANRPlayerControllerBase::OpenOrCloseMenu()
{
	if (UIContainer && !bAlreadyFinished)
	{
		if (UIContainer->IsMenuOpened())
		{
			SetInputMode(FInputModeGameOnly{});
			bShowMouseCursor = false;
			UIContainer->ReceiveOnCloseMenu();
		}
		else
		{
			SetInputMode(FInputModeGameAndUI{});
			bShowMouseCursor = true;
			UIContainer->ReceiveOnOpenMenu();
		}
	}
}

void ANRPlayerControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (UIContainer && GetWorld())
	{
		ANRPlayerStateBase* pPlayerState = GetPlayerState<ANRPlayerStateBase>();
		AGameState* pGameState = GetWorld()->GetGameState<AGameState>();

		if (pPlayerState && pGameState)
		{
			UIContainer->ReceiveUpdateLapInfo(pPlayerState->PlayerPlace, pGameState->PlayerArray.Num(), pPlayerState->CurrentLap);
		}
	}
}

void ANRPlayerControllerBase::OnRep_CurrentItem()
{
	ReceivedPickUpItem();
}

void ANRPlayerControllerBase::ReceivedPickUpItem()
{
	if (UIContainer)
	{
		if (CurrentItem)
		{
			UIContainer->ReceiveItemPickedUp(CurrentItem);
		}
		else
		{
			UIContainer->ReceiveItemRemoved();
		}
	}
}

#pragma endregion