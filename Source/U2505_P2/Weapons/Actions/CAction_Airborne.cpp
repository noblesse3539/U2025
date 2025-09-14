#include "Weapons/Actions/CAction_Airborne.h"
#include "Global.h"
#include "Weapons/CDoAction.h"

UCAction_Airborne::UCAction_Airborne()
{
}


void UCAction_Airborne::Pressed()
{
	Super::Pressed();

	CheckNull(DoAction);
	DoAction->DoAction();
}

