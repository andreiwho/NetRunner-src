// Copyleft, Andrey Protsenko. No rights reserved.

#include "Game/Platforms/NRJumpPlatform.h"
#include "Components/ArrowComponent.h"
#include "Character/NRCharacterBase.h"
#include "DrawDebugHelpers.h"

ANRJumpPlatform::ANRJumpPlatform()
{
	LaunchDirectionArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LaunchDirectionArrow"));
	LaunchDirectionArrow->SetupAttachment(GetRootComponent());
}

FVector ANRJumpPlatform::GetLaunchDirectionVector() const
{
	const FVector Forward = GetActorForwardVector();
	const FVector Right = GetActorRightVector();
	const FVector OutDirectionVector = Forward.RotateAngleAxis(-LaunchAngle, Right);
	return OutDirectionVector;
}

void ANRJumpPlatform::ApplyModifier(ANRCharacterBase* Character)
{
	Super::ApplyModifier(Character);

	Character->LaunchCharacter(GetLaunchDirectionVector() * LaunchVelocity, bOverrideXYVelocity, bOverrideZVelocity);
}

void ANRJumpPlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	if (LaunchDirectionArrow)
	{
		LaunchDirectionArrow->SetWorldRotation(GetLaunchDirectionVector().Rotation());
	}
#endif
}
