// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NRPublicTypes.h"
#include "NRGameMode_Common.generated.h"

class ANRPlayerControllerBase;
class ANRLapSpline;
class ANRPlayerStateBase;

UCLASS()
class NETRUNNER_API ANRGameMode_Common : public AGameMode
{
	GENERATED_BODY()

public:
	ANRGameMode_Common();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
	void TeleportPlayerToLastCheckpoint(ANRPlayerControllerBase* pPlayerController);

	virtual void OnMatchStateSet() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Race")
	bool bHasLaps = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Race", meta=(EditCondition="bHasLaps"))
	int32 LapCount = 3;

	UPROPERTY()
	ANRLapSpline* LapSplineActor;

	void PlayerFinished(APlayerState* PlayerState);

	void OnTravelTimerFinished();

	UPROPERTY(VisibleAnywhere)
	ENRMapType NextMap;

private:
	void CollectPlayerLapInfo();
	void CalculatePlayerPlaces();

	TArray<ANRPlayerStateBase*> InGamePlaces;
	int32 FinishedPlayerCount = 0;

	FTimerHandle TravelTimer;
};
