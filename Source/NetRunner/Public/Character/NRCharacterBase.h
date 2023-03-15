// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NRCharacterBase.generated.h"

class UCameraComponent;
class ANRPlayerControllerBase;
class USoundCue;

UCLASS(config=Game)
class ANRCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ANRCharacterBase();

	virtual void BeginPlay() override;

	void SetSpeedModifier(float InModifier);

	UFUNCTION(Server, Reliable)
	void UpdateDefaultSpeedModifier(float NewModifier);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ANRPlayerControllerBase* GetNRPlayerController() const { return LocalPlayerController; }

	void UseCurrentItem();

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);
	
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(Replicated)
	float DefaultSpeedModifier = 1.0f;

	UPROPERTY(Replicated)
	float CurrentSpeedModifier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpeedUp")
	float ModifierDecreaseSpeed = 10.0f;

	float Cached_MaxWalkSpeed;

	virtual void FellOutOfWorld(const class UDamageType& DmgType) override;

	void UpdatePlayerUI();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_Controller() override;

	void ReceivedPlayerController();

	void OnMenuPressed();

	UPROPERTY(EditAnywhere, Category="SFX")
	USoundCue* WindSound;

	UPROPERTY()
	UAudioComponent* WindComponent;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY()
	ANRPlayerControllerBase* LocalPlayerController;
};

