// Copyleft, Andrey Protsenko. No rights reserved.


#include "Game/Platforms/NRFinishPlatform.h"
#include "Character/NRCharacterBase.h"
#include "Player/NRPlayerStateBase.h"
#include "Components/ArrowComponent.h"

ANRFinishPlatform::ANRFinishPlatform()
{
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
}

void ANRFinishPlatform::ApplyModifier(ANRCharacterBase* Character)
{
	if (!Character || !Arrow)
	{
		return;
	}

	const FVector ArrowForward = Arrow->GetForwardVector();
	const FVector CharacterMovementDir = Character->GetVelocity().GetSafeNormal();
	const float Dot = FVector::DotProduct(ArrowForward, CharacterMovementDir);
	UE_LOG(LogTemp, Warning, TEXT("Dot product: %f"), Dot);

	if (ANRPlayerStateBase* pPlayerState = Character->GetPlayerState<ANRPlayerStateBase>())
	{
		pPlayerState->IncrementLap();
		pPlayerState->BlockIncrementLap(Dot < 0.0f);
	}
}
