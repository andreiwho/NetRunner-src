// Copyleft, Andrey Protsenko. No rights reserved.


#include "Game/NRCheckPointBase.h"
#include "Components/BoxComponent.h"
#include "Character/NRCharacterBase.h"
#include "Controller/NRPlayerControllerBase.h"

// Sets default values
ANRCheckPointBase::ANRCheckPointBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	if (BoxCollision)
	{
		SetRootComponent(BoxCollision);
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		BoxCollision->SetBoxExtent(FVector(500.0f, 500.0f, 500.0f));
	}
}

void ANRCheckPointBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANRCharacterBase* pCharacter = Cast<ANRCharacterBase>(OtherActor);
	if (!pCharacter)
	{
		return;
	}

	ANRPlayerControllerBase* pPlayerController = pCharacter->GetNRPlayerController();
	if(!ensure(pPlayerController))
	{
		return;
	}

	pPlayerController->LastCheckpoint = this;
}

// Called when the game starts or when spawned
void ANRCheckPointBase::BeginPlay()
{
	Super::BeginPlay();
	check(BoxCollision);

	if (HasAuthority())
	{
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ANRCheckPointBase::OnComponentBeginOverlap);
	}
}

FTransform ANRCheckPointBase::GetRestartTransform() const
{
	return GetTransform();
}
