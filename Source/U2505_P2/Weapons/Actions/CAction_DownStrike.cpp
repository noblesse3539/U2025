#include "Weapons/Actions/CAction_DownStrike.h"
#include "Global.h"
#include "Weapons/CDoAction.h"

UCAction_DownStrike::UCAction_DownStrike()
{
}

void UCAction_DownStrike::Pressed()
{
	Super::Pressed();

	CheckNull(DoAction);
	DoAction->DoAction();
}
