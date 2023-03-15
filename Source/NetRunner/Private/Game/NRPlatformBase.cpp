// Copyleft, Andrey Protsenko. No rights reserved.

#include "Game/NRPlatformBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Character/NRCharacterBase.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

ANRPlatformBase::ANRPlatformBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	if (BoxCollision)
	{
		SetRootComponent(BoxCollision);
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	}

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	if (StaticMesh)
	{
		StaticMesh->SetupAttachment(GetRootComponent());
		StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}

void ANRPlatformBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ANRPlatformBase::OnBeginOverlap);
	}
}

void ANRPlatformBase::ApplyModifier(ANRCharacterBase* Character)
{
	if (!ApplySound || !GetWorld())
	{
		return;
	}

	// We want everyone to hear how good we are
	if (ShouldPlayApplySound())
	{
		Multicast_PlayApplySound();
	}
}

void ANRPlatformBase::Multicast_PlayApplySound_Implementation()
{
	if (UWorld* pWorld = GetWorld())
	{
		if (ApplySound)
		{
			UGameplayStatics::SpawnSoundAtLocation(pWorld, ApplySound, GetActorLocation());
		}
	}
}

bool ANRPlatformBase::ShouldPlayApplySound() const
{
	return true;
}

void ANRPlatformBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANRCharacterBase* pCharacter = Cast<ANRCharacterBase>(OtherActor);
	if (!pCharacter)
	{
		return;
	}

	ApplyModifier(pCharacter);
}

