#include "Weapons/Actions/CAction_Defend.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"

#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"

UCAction_Defend::UCAction_Defend()
{
}

void UCAction_Defend::BeginDestroy()
{

	if (OwnerCharacter.IsValid())
	{
		FHelpers::ClearTimer(OwnerCharacter->GetWorld(), Handle_Parry);
	}
	Super::BeginDestroy();
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
			if (OwnerCharacter.IsValid())
			{
				FTimerDelegate parryTimer = FTimerDelegate::CreateWeakLambda(this, [this]() {
					ExpireParry();
				});

				UWorld* world = OwnerCharacter->GetWorld();
				if (!IsValid(world))
					break;

				world->GetTimerManager().SetTimer(Handle_Parry, parryTimer, ParryDuration, false);
				break;
			}
		}
	}

	Super::OnStateTypeChanged(InPrevType, InNewType);
}

void UCAction_Defend::OnDefend()
{
	CheckNotValid(State);

	State->SetDefendMode();
}

void UCAction_Defend::OffDefend()
{
	CheckNotValid(State);

	State->SetIdleMode();
}

void UCAction_Defend::ExpireParry()
{
	bParry = false;
}
