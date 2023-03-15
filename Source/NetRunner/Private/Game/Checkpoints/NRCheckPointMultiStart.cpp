// Copyleft, Andrey Protsenko. No rights reserved.


#include "Game/Checkpoints/NRCheckPointMultiStart.h"

void ANRCheckPointMultiStart::RegisterComponentTransform(USceneComponent* Component)
{
	Transforms.Add(Component->GetComponentTransform());
}

FTransform ANRCheckPointMultiStart::GetRestartTransform() const
{
	if (Transforms.Num() <= 0)
	{
		return GetTransform();
	}
	return Transforms[FMath::RandRange(0, Transforms.Num() - 1)];
}
