#include "Weapons/Actions/CAction_Action.h"
#include "Global.h"
#include "Weapons/CDoAction.h"


void UCAction_Action::Tick(float InDeltaTime)
{
	CheckNull(DoAction);

	DoAction->Tick(InDeltaTime);
}

void UCAction_Action::Pressed()
{
	Super::Pressed();

	CheckNull(DoAction);
	DoAction->DoAction();
}

void UCAction_Action::Released()
{
	Super::Released();

	CheckNull(DoAction);
	DoAction->End_DoAction();
}
