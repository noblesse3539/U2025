#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

#include "Weapons/CWeaponAsset.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Weapons/DoActions/CDoAction_DownStrike.h"
#include "Weapons/DoActions/CDoAction_Finish.h"

#include "Weapons/CActionBase.h"
#include "Weapons/Actions/CAction_Action.h"
#include "Weapons/Actions/CAction_Defend.h"
#include "Weapons/Actions/CAction_Air.h"
#include "Weapons/Actions/CAction_DownStrike.h"
#include "Weapons/Actions/CAction_Airborne.h"
#include "Weapons/Actions/CAction_Finish.h"
#include "Weapons/Actions/CAction_Skill.h"

#include "Characters/CPlayer.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	for (TPair<EWeaponType, class UCWeaponAsset*> asset : DataAssetTable)
	{
		if (asset.Value == nullptr)
		{
			continue;
		}
		FWeaponData data;
		asset.Value->BeginPlay(this, OwnerCharacter, data);
		WeaponDataMap.Add(asset.Key, data);
	}

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state);
	state->OnStateTypeChanged.AddDynamic(this, &UCWeaponComponent::OnStateTypeChanged);
}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCharge)
	{
		ChargingTime += DeltaTime;
		if (ChargingTime > ChargeThreshold) // TODO 공중에선 false 처리
		{
			DoAction_Attack();
			OffCharge();
		}
	}

	UCActionBase* action = GetAction();
	if (IsValid(action))
	{
		action->Tick(DeltaTime);
	}
}

bool UCWeaponComponent::IsParry()
{
	UCAction_Defend* defend = GetAction_Defend();
	CheckNullResult(defend, false);

	return defend->IsParry();
}

UCWeaponAsset* UCWeaponComponent::GetWeaponAsset(EWeaponType InType)
{
	UCWeaponAsset** asset = DataAssetTable.Find(InType);
	CheckNullResult(asset, nullptr);

	return *asset;
}

ACAttachment* UCWeaponComponent::GetAttachment(EWeaponType InType)
{
	// CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(WeaponDataMap.Contains(InType), nullptr);

	return WeaponDataMap.Find(InType)->Attachment;
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	return GetAttachment(CurrentType);
}

UCEquipment* UCWeaponComponent::GetEquipment(EWeaponType InType)
{
	CheckFalseResult(WeaponDataMap.Contains(InType), nullptr);

	return WeaponDataMap.Find(InType)->Equipment;
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	return GetEquipment(CurrentType);
}

UCActionBase* UCWeaponComponent::GetAction(EWeaponType InType, EActionType InActionType)
{
	CheckFalseResult(WeaponDataMap.Contains(InType), nullptr);

	switch (InActionType)
	{
		case EActionType::Action:
			return GetAction_Action(InType);

		case EActionType::Air:
			return GetAction_Air(InType);

		case EActionType::DownStrike:
			return GetAction_DownStrike(InType);

		case EActionType::Airborne:
			return GetAction_Airborne(InType);

		case EActionType::Finish:
			return GetAction_Finish(InType);

		case EActionType::Skill:
			return GetAction_Skill(InType);

		case EActionType::Defend:
			return GetAction_Defend(InType);
	}

	return nullptr;
}

UCActionBase* UCWeaponComponent::GetAction()
{
	return GetAction(CurrentType, CurrentActionType);
}

UCAction_Action* UCWeaponComponent::GetAction_Action(EWeaponType InType)
{
	CheckFalseResult(WeaponDataMap.Contains(InType), nullptr);

	return WeaponDataMap.Find(InType)->Action;
}

UCAction_Action* UCWeaponComponent::GetAction_Action()
{
	return GetAction_Action(CurrentType);
}

UCAction_Defend* UCWeaponComponent::GetAction_Defend(EWeaponType InType)
{
	CheckFalseResult(WeaponDataMap.Contains(InType), nullptr);

	return WeaponDataMap.Find(InType)->Defend;
}

UCAction_Defend* UCWeaponComponent::GetAction_Defend()
{
	return GetAction_Defend(CurrentType);
}

UCAction_Air* UCWeaponComponent::GetAction_Air(EWeaponType InType)
{
	CheckFalseResult(WeaponDataMap.Contains(InType), nullptr);

	return WeaponDataMap.Find(InType)->Action_Air;
}

UCAction_Air* UCWeaponComponent::GetAction_Air()
{
	return GetAction_Air(CurrentType);
}

UCAction_DownStrike* UCWeaponComponent::GetAction_DownStrike(EWeaponType InType)
{
	CheckFalseResult(WeaponDataMap.Contains(InType), nullptr);

	return WeaponDataMap.Find(InType)->Action_DownStrike;
}

UCAction_DownStrike* UCWeaponComponent::GetAction_DownStrike()
{
	return GetAction_DownStrike(CurrentType);
}

UCAction_Airborne* UCWeaponComponent::GetAction_Airborne(EWeaponType InType)
{
	CheckFalseResult(WeaponDataMap.Contains(InType), nullptr);

	return WeaponDataMap.Find(InType)->Action_Airborne;
}

UCAction_Airborne* UCWeaponComponent::GetAction_Airborne()
{
	return GetAction_Airborne(CurrentType);
}

UCAction_Finish* UCWeaponComponent::GetAction_Finish(EWeaponType InType)
{
	CheckFalseResult(WeaponDataMap.Contains(InType), nullptr);

	return WeaponDataMap.Find(InType)->Action_Finish;
}

UCAction_Finish* UCWeaponComponent::GetAction_Finish()
{
	return GetAction_Finish(CurrentType);
}

UCAction_Skill* UCWeaponComponent::GetAction_Skill(EWeaponType InType)
{
	CheckFalseResult(WeaponDataMap.Contains(InType), nullptr);

	return WeaponDataMap.Find(InType)->Action_Skill;
}

UCAction_Skill* UCWeaponComponent::GetAction_Skill()
{
	return GetAction_Skill(CurrentType);
}

void UCWeaponComponent::Action_Action_Pressed()
{
	UCAction_Action* action = GetAction_Action();
	CheckNull(action);

	action->Pressed();
}

void UCWeaponComponent::Action_Defend_Pressed()
{
	UCAction_Defend* defend = GetAction_Defend();
	CheckNull(defend);

	defend->Pressed();
}

void UCWeaponComponent::Action_Defend_Released()
{
	UCAction_Defend* defend = GetAction_Defend();
	CheckNull(defend);

	defend->Released();
}

void UCWeaponComponent::Action_Air_Pressed()
{
	UCAction_Air* air = GetAction_Air();
	CheckNull(air);

	air->Pressed();
}

void UCWeaponComponent::Action_DownStrike_Pressed()
{
	UCAction_DownStrike* downStrike = GetAction_DownStrike();
	CheckNull(downStrike);

	downStrike->Pressed();
}

void UCWeaponComponent::Action_Airborne_Pressed()
{
	UCAction_Airborne* airborne = GetAction_Airborne();
	CheckNull(airborne);

	airborne->Pressed();
}

void UCWeaponComponent::Action_Finish_Pressed()
{
	UCAction_Finish* finish = GetAction_Finish();
	CheckNull(finish);

	finish->Pressed();
}

void UCWeaponComponent::Action_Skill_Pressed()
{
	UCAction_Skill* skill = GetAction_Skill();
	CheckNull(skill);

	skill->Pressed();
}

bool UCWeaponComponent::IsIdleMode()
{
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNullResult(state, false);

	return state->IsIdleMode();
}

void UCWeaponComponent::SetUnarmedMode()
{
	CheckTrue(IsUnarmedMode());

	UCEquipment* equipment = GetEquipment();
	if (IsValid(equipment))
		equipment->Unequip();

	ChangeType(EWeaponType::Max);

	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	CheckNull(movement);
	movement->OffFixForward();
}

void UCWeaponComponent::SetFirstWeaponMode()
{
	TArray<EWeaponType> keys;
	DataAssetTable.GetKeys(keys);

	CheckFalse(keys.Num() > 0);

	switch (keys[0])
	{
		case EWeaponType::Fist:
		{
			break;
		}
		case EWeaponType::Sword:
		{
			SetSwordMode();
			break;
		}
		case EWeaponType::Hammer:
		{
			break;
		}
		case EWeaponType::Axe:
		{
			SetAxeMode();
			break;
		}
	}
}

void UCWeaponComponent::SetFistMode()
{
	CheckTrue(IsUnarmedMode());

	SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetSwordMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetAxeMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Axe);
}

void UCWeaponComponent::SetKatanaMode()
{
	SetMode(EWeaponType::Katana);
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{

	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	CheckNull(movement);
	if (InType != EWeaponType::Katana)
	{
		movement->OnFixForward();
	}

	if (CurrentType == InType)
	{
		SetUnarmedMode();
		return;
	}

	UCEquipment* equipment = GetEquipment();

	if (IsUnarmedMode() == false)
	{
		if (IsValid(equipment))
			equipment->Unequip();
	}

	bEquipping = true;

	equipment = GetEquipment(InType);
	if (IsValid(equipment))
		equipment->Equip();

	ChangeType(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType type = CurrentType;
	CurrentType = InType;

	if (OnWeaponTypeChanged.IsBound())
	{
		OnWeaponTypeChanged.Broadcast(type, InType);
	}
}

void UCWeaponComponent::SetAction_Default()
{
	SetActionType(EActionType::Max);
}

void UCWeaponComponent::SetActionType(EActionType InType)
{
	ChangeActionType(InType);
}

void UCWeaponComponent::ChangeActionType(EActionType InType)
{
	CheckFalse(CanChangeActionType(CurrentActionType, InType));

	EActionType type = CurrentActionType;
	CurrentActionType = InType;

	if (OnActionTypeChanged.IsBound())
	{
		OnActionTypeChanged.Broadcast(type, InType);
	}
}

bool UCWeaponComponent::CanChangeActionType(EActionType InFromType, EActionType InToType)
{

	switch (InFromType)
	{
		case EActionType::Action:
		case EActionType::Air:
		case EActionType::DownStrike:
		case EActionType::Airborne:
		case EActionType::Finish:
		{
			if (InToType == EActionType::Defend)
				return false;
			break;
		}
		case EActionType::Defend:
		{
			return InToType == EActionType::Max;
		}
	}

	return true;
}

void UCWeaponComponent::DoAction()
{
	UCActionBase* action = GetAction();
	CheckNull(action);

	action->Pressed();
}

void UCWeaponComponent::EndDoAction()
{
	UCActionBase* action = GetAction();
	CheckNull(action);

	action->Released();
}

void UCWeaponComponent::DoAction_Attack()
{
	CheckNull(OwnerCharacter);

	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	CheckNull(movement);

	CheckTrue(IsUnarmedMode());

	// 차지 완료 시 에어본 공격
	if (ChargingTime >= ChargeThreshold)
	{
		EndDoAction(); // 기존 콤보 초기화
		Action_Airborne_Pressed();
	}
	else
	{
		OnCharge();

		if (movement->IsFalling())
		{
			Action_DownStrike_Pressed();
		}
		else
		{
			Action_Action_Pressed();
		}
	}

	// DoAction();
}

void UCWeaponComponent::Begin_Equip()
{
	UCEquipment* equipment = GetEquipment();
	CheckFalse(IsValid(equipment));

	equipment->Begin_Equip();
}

void UCWeaponComponent::End_Equip()
{
	UCEquipment* equipment = GetEquipment();
	CheckFalse(IsValid(equipment));

	equipment->End_Equip();
	bEquipping = false;
}

void UCWeaponComponent::OnCharge()
{
	bCharge = true;
}

void UCWeaponComponent::OffCharge()
{
	bCharge = false;
	ChargingTime = 0.0f;
}

void UCWeaponComponent::Begin_DownStrike()
{
	UCAction_DownStrike* downStrike = GetAction_DownStrike();
	CheckNull(downStrike);
	UCDoAction_DownStrike* downAction = Cast<UCDoAction_DownStrike>(downStrike->GetDoAction());
	CheckNull(downAction);

	downAction->Begin_DoAction();
}

void UCWeaponComponent::AttachTo(FName InSocketName)
{
	GetAttachment()->AttachTo(InSocketName);
}

void UCWeaponComponent::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
}
