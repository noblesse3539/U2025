#include "Weapons/Actions/CAction_Defend.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"

UCAction_Defend::UCAction_Defend()
{
}

void UCAction_Defend::Pressed()
{
	Super::Pressed();

	OnDefend();
}

void UCAction_Defend::Released()
{
	Super::Released();

	OffDefend();
}

void UCAction_Defend::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InPrevType)
	{
	case EStateType::Defend:
	{
		bParry = false;
		break;
	}
	}

	switch (InNewType)
	{
		case EStateType::Defend:
		{
			bParry = true;

			FTimerDelegate parryTimer = FTimerDelegate::CreateUObject(this, &UCAction_Defend::ExpireParry);
			GetWorld()->GetTimerManager().SetTimer(Handle_Parry, parryTimer, ParryDuration, false);
			break;
		}
	}

	Super::OnStateTypeChanged(InPrevType, InNewType);
}

void UCAction_Defend::OnDefend()
{
	CheckNull(State);

	State->SetDefendMode();
}

void UCAction_Defend::OffDefend()
{
	CheckNull(State);

	State->SetIdleMode();
}

void UCAction_Defend::ExpireParry()
{
	bParry = false;
}
