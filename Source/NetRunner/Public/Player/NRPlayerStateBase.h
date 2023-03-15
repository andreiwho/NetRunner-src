// Copyleft, Andrey Protsenko. No rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NRPlayerStateBase.generated.h"

class ANRLapSpline;

UCLASS()
class NETRUNNER_API ANRPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;

	void IncrementLap();

	void UpdateLapPosition(float NewPosition);

	UPROPERTY(ReplicatedUsing=OnRep_CurrentLap, VisibleAnywhere, BlueprintReadOnly, Category="Lap")
	int32 CurrentLap = 0;

	UFUNCTION()
	void OnRep_CurrentLap();

	void ReceiveCurrentLap();

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Lap")
	float LapPosition = 0.0f;

	// Which place are we on right now
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Lap")
	int32 PlayerPlace = 0;

	FORCEINLINE void BlockIncrementLap(bool bBlock) { bIncrementBlocked = bBlock; }

	FORCEINLINE bool IsIncrementBlocked() const { return bIncrementBlocked; }

	FORCEINLINE bool HasPlayerFinsihed() const { return bHasFinished; }

private:
	// We can increment the lap if and only if the ((CurrentLap - 1) + LapPosition > (CurrentLap - 1 + CheckIncrementThreshold)
	float CheckIncrementThreshold = 0.8f;

	FORCEINLINE bool CanIncrementLap() const { return (LapPosition >  CurrentLap + CheckIncrementThreshold) && !IsIncrementBlocked(); }

	bool bIncrementBlocked = false;

	bool bHasFinished = false;
};
