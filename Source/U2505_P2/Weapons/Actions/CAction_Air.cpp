#include "Weapons/Actions/CAction_Air.h"
#include "Global.h"
#include "Weapons/CDoAction.h"

UCAction_Air::UCAction_Air()
{
}

void UCAction_Air::Tick(float InDeltaTime)
{
	CheckNull(DoAction);

	DoAction->Tick(InDeltaTime);
}

void UCAction_Air::Pressed()
{
	Super::Pressed();

	CheckNull(DoAction);
	DoAction->DoAction();
}
