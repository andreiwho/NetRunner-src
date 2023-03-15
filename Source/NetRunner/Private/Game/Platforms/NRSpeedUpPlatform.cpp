// Copyleft, Andrey Protsenko. No rights reserved.


#include "Game/Platforms/NRSpeedUpPlatform.h"
#include "Character/NRCharacterBase.h"

void ANRSpeedUpPlatform::ApplyModifier(ANRCharacterBase* Character)
{
	Super::ApplyModifier(Character);

	if (Character->HasAuthority())
	{
		Character->SetSpeedModifier(SpeedUpModifier);
	}
}
