#include "Weapons/CActionBase.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponTraceComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"

UCActionBase::UCActionBase()
{
}

void UCActionBase::BeginPlay(ACharacter* InOwnerCharacter, ACAttachment* InAttachment, UCEquipment* InEquipment, const EActionType InActionType)
{
	OwnerCharacter = InOwnerCharacter;

	Attachment = InAttachment;

	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter.Get());

	State->OnStateTypeChanged.AddDynamic(this, &UCActionBase::OnStateTypeChanged);

	if (!!DoActionClass)
	{

		UCWeaponTraceComponent* weaponTrace = nullptr;
		if (!!Attachment)
		{
			weaponTrace = FHelpers::GetComponent<UCWeaponTraceComponent>(Attachment);
		}

		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(InOwnerCharacter, InAttachment, InEquipment, weaponTrace, DoActionDatas, DamagedDatas, InActionType);
	}
}

void UCActionBase::BeginDestroy()
{
	if (State.IsValid())
	{
		State->OnStateTypeChanged.RemoveDynamic(this, &UCActionBase::OnStateTypeChanged);
	}

	Super::BeginDestroy();
}

void UCActionBase::Pressed()
{
	bInAction = true;
}

void UCActionBase::Released()
{
	bInAction = false;
}
