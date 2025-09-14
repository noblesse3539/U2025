#include "Weapons/Actions/CAction_Finish.h"
#include "Global.h"
#include "Weapons/CDoAction.h"

UCAction_Finish::UCAction_Finish()
{
}

void UCAction_Finish::Tick(float InDeltaTime)
{
	CheckNull(DoAction);

	DoAction->Tick(InDeltaTime);
}

void UCAction_Finish::Pressed()
{
	Super::Pressed();

	CheckNull(DoAction);
	DoAction->DoAction();
}
