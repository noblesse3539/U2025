#include "Components/CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{
}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetEvadeMode()
{
	ChangeType(EStateType::Evade);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetDamagedMode()
{
	ChangeType(EStateType::Damaged);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetKnockdownMode()
{
	ChangeType(EStateType::Knockdown);
}

void UCStateComponent::SetGetupMode()
{
	ChangeType(EStateType::Getup);
}

void UCStateComponent::SetDefendMode()
{
	ChangeType(EStateType::Defend);
}

void UCStateComponent::ChangeType(EStateType InType)
{
	// CheckFalse(CanChangeType(Current, InType));

	EStateType type = Current;
	Current = InType;

	if (OnStateTypeChanged.IsBound())
	{
		OnStateTypeChanged.Broadcast(type, InType);
	}
}

bool UCStateComponent::CanChangeType(EStateType InFromType, EStateType InToType)
{
	switch (InFromType)
	{
		case EStateType::Knockdown: // 넉다운상태에서는 겟업으로만 변경가능
		{
			return InToType == EStateType::Getup;
		}
	}

	return true;
}
