// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/NRCharacterBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameMode/NRGameMode_Common.h"
#include "Controller/NRPlayerControllerBase.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

ANRCharacterBase::ANRCharacterBase()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	PrimaryActorTick.bCanEverTick = true;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 1200.0f;
	GetCharacterMovement()->AirControl = 0.4f;
	Cached_MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(GetRootComponent());
	FollowCamera->bUsePawnControlRotation = true;

	if (USkeletalMeshComponent* pMesh = GetMesh())
	{
		pMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		pMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	}
}

void ANRCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (WindSound)
	{
		WindComponent = UGameplayStatics::SpawnSound2D(this, WindSound, 0.0f, 1.0f, 0.0f, nullptr, false, true);
	}
}

void ANRCharacterBase::SetSpeedModifier(float InModifier)
{
	CurrentSpeedModifier = InModifier;
}

void ANRCharacterBase::UpdateDefaultSpeedModifier_Implementation(float NewModifier)
{
	DefaultSpeedModifier = NewModifier;
}

void ANRCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANRCharacterBase, DefaultSpeedModifier);
	DOREPLIFETIME(ANRCharacterBase, CurrentSpeedModifier);
}

void ANRCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!FMath::IsNearlyEqual(CurrentSpeedModifier, DefaultSpeedModifier))
	{
		CurrentSpeedModifier = FMath::Lerp(CurrentSpeedModifier, DefaultSpeedModifier, DeltaSeconds * ModifierDecreaseSpeed);
	}
	GetCharacterMovement()->MaxWalkSpeed = Cached_MaxWalkSpeed * CurrentSpeedModifier;

	// Update UI
	if (IsLocallyControlled())
	{
		UpdatePlayerUI();
	}

	if (WindComponent)
	{
		const float CurrentSpeedFactor = GetCharacterMovement()->Velocity.Size() / (Cached_MaxWalkSpeed * 4.0f);
		WindComponent->SetVolumeMultiplier(CurrentSpeedFactor <= 0.1f ? 0.1f : CurrentSpeedFactor);
	}
}

void ANRCharacterBase::UseCurrentItem()
{
	if (IsLocallyControlled())
	{
		if (ANRPlayerControllerBase* pPlayerController = GetNRPlayerController())
		{
			pPlayerController->Server_UseCurrentItem();
		}
	}
}

void ANRCharacterBase::FellOutOfWorld(const class UDamageType& DmgType)
{
	if (HasAuthority())
	{
		if (UWorld* pWorld = GetWorld())
		{
			if (ANRGameMode_Common* pGameMode = pWorld->GetAuthGameMode<ANRGameMode_Common>())
			{
				pGameMode->TeleportPlayerToLastCheckpoint(GetNRPlayerController());
			}
		}
	}
}

void ANRCharacterBase::UpdatePlayerUI()
{
	if (ANRPlayerControllerBase* pPlayerController = GetNRPlayerController())
	{
		pPlayerController->UI_UpdateCharacterSpeedModifier(CurrentSpeedModifier);
	}
}

void ANRCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ReceivedPlayerController();
}

void ANRCharacterBase::OnRep_Controller()
{
	Super::OnRep_Controller();

	ReceivedPlayerController();
}

void ANRCharacterBase::ReceivedPlayerController()
{
	LocalPlayerController = Cast<ANRPlayerControllerBase>(Controller);
	check(LocalPlayerController);
}

void ANRCharacterBase::OnMenuPressed()
{
	if (IsLocallyControlled())
	{
		if (ANRPlayerControllerBase* pPlayerController = GetNRPlayerController())
		{
			pPlayerController->OpenOrCloseMenu();
		}
	}
}

void ANRCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &ANRCharacterBase::UseCurrentItem);
	PlayerInputComponent->BindAction("Menu", IE_Pressed, this, &ANRCharacterBase::OnMenuPressed);
	PlayerInputComponent->BindAxis("MoveForward", this, &ANRCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ANRCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ANRCharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ANRCharacterBase::LookUpAtRate);
}

void ANRCharacterBase::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ANRCharacterBase::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ANRCharacterBase::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ANRCharacterBase::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
