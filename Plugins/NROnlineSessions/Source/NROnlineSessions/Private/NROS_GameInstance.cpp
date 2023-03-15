// Copyleft, Andrey Protsenko. No rights reserved.

#include "NROS_GameInstance.h"
#include "NROS_SessionController.h"

void UNROS_GameInstance::Init()
{
	Super::Init();

	SessionController = NewObject<UNROS_SessionController>(this);
	check(SessionController);
}
