// Copyleft, Andrey Protsenko. No rights reserved.

#include "Game/Items/NRTriggeredItem.h"
#include "Components/SphereComponent.h"
#include "Character/NRCharacterBase.h"

ANRTriggeredItem::ANRTriggeredItem()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	if (SphereCollision)
	{
		SetRootComponent(SphereCollision);
		SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
}

void ANRTriggeredItem::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		check(SphereCollision);
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ANRTriggeredItem::OnBeginOverlap);
	}
}

void ANRTriggeredItem::TriggeredByPawn(ANRCharacterBase* Character)
{
}

void ANRTriggeredItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ANRCharacterBase* pCharacter = Cast<ANRCharacterBase>(OtherActor))
	{
		if (GetOwner() == pCharacter)
		{
			return;
		}

		TriggeredByPawn(pCharacter);
		if (bDestroyOnTrigger)
		{
			Destroy();
		}

	}
}
