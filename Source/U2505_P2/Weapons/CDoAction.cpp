#include "Weapons/CDoAction.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CMovementComponent.h"

#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"

#include "GenericTeamAgentInterface.h"

void UCDoAction::BeginPlay(ACharacter* InOwnerCharacter, ACAttachment* InAttachment, UCEquipment* InEquipment, const TArray<FDoActionData> InDatas, const TArray<FDamagedData> InDamagedDatas, const EActionType InActionType)
{
	OwnerCharacter = InOwnerCharacter;
	World = OwnerCharacter->GetWorld();
	Datas = InDatas;
	DamagedDatas = InDamagedDatas;
	ActionType = InActionType;

	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
}

void UCDoAction::DoAction()
{
	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(weapon);
	weapon->SetActionType(ActionType);
	State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
}

void UCDoAction::End_DoAction()
{

	State->SetIdleMode();

	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	if (!!movement)
	{
		movement->Move();
	}

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(weapon);
	weapon->SetAction_Default();
}

bool UCDoAction::IsValidActionType()
{
	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNullResult(weapon, false);

	CheckFalseResult(weapon->GetActionType() == ActionType, false);

	return true;
}

// 다른 팀인 경우 true
bool UCDoAction::IsValidTeam(ACharacter* InOther)
{
	IGenericTeamAgentInterface* iTeamAttacker = Cast<IGenericTeamAgentInterface>(OwnerCharacter);
	IGenericTeamAgentInterface* iTeamOther = Cast<IGenericTeamAgentInterface>(InOther);

	if (iTeamAttacker != nullptr && iTeamOther != nullptr)
	{
		CheckTrueResult(iTeamAttacker->GetGenericTeamId() == iTeamOther->GetGenericTeamId(), false);
	}
	return true;
}
