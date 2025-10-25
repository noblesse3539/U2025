#include "Weapons/CDoAction.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponTraceComponent.h"

#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"

#include "GenericTeamAgentInterface.h"

void UCDoAction::BeginDestroy()
{
	if (Attachment.IsValid())
	{
		Attachment.Get()->OnAttachmentBeginCollision.RemoveDynamic(this, &UCDoAction::OnAttachmentBeginCollision);
		Attachment.Get()->OnAttachmentEndCollision.RemoveDynamic(this, &UCDoAction::OnAttachmentEndCollision);
		Attachment.Get()->OnAttachmentBeginOverlap.RemoveDynamic(this, &UCDoAction::OnAttachmentBeginOverlap);
		Attachment.Get()->OnAttachmentEndOverlap.RemoveDynamic(this, &UCDoAction::OnAttachmentEndOverlap);
	}

	if (WeaponTrace.IsValid())
	{
		WeaponTrace.Get()->OnWeaponTraceBeginOverlap.RemoveDynamic(this, &UCDoAction::OnAttachmentBeginOverlap);
	}

	Super::BeginDestroy();
}

void UCDoAction::BeginPlay(ACharacter* InOwnerCharacter, ACAttachment* InAttachment, UCEquipment* InEquipment, UCWeaponTraceComponent* InWeaponTrace, const TArray<FDoActionData> InDatas, const TArray<FDamagedData> InDamagedDatas, const EActionType InActionType)
{
	OwnerCharacter = InOwnerCharacter;
	Attachment = InAttachment;
	Equipment = InEquipment;
	WeaponTrace = InWeaponTrace;

	World = OwnerCharacter->GetWorld();

	Datas = InDatas;
	DamagedDatas = InDamagedDatas;
	ActionType = InActionType;

	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter.Get());

	// Bind Delegate
	if (Attachment.IsValid())
	{
		Attachment->OnAttachmentBeginCollision.AddDynamic(this, &UCDoAction::OnAttachmentBeginCollision);
		Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCDoAction::OnAttachmentEndCollision);
		Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCDoAction::OnAttachmentBeginOverlap);
		Attachment->OnAttachmentEndOverlap.AddDynamic(this, &UCDoAction::OnAttachmentEndOverlap);
	}
	if (WeaponTrace.IsValid())
	{
		WeaponTrace->OnWeaponTraceBeginOverlap.AddDynamic(this, &UCDoAction::OnAttachmentBeginOverlap);
	}
}

void UCDoAction::DoAction()
{
	CheckNotValid(OwnerCharacter);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter.Get());
	CheckNull(weapon);

	weapon->SetActionType(ActionType);

	CheckNotValid(State);
	State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
}

void UCDoAction::End_DoAction()
{
	CheckNotValid(OwnerCharacter);
	CheckNotValid(State);

	State->SetIdleMode();

	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter.Get());
	if (!!movement)
	{
		movement->Move();
	}

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter.Get());
	CheckNull(weapon);
	weapon->SetAction_Default();
}

bool UCDoAction::IsValidActionType()
{
	CheckNotValidResult(OwnerCharacter, false);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter.Get());
	CheckNullResult(weapon, false);

	CheckFalseResult(weapon->GetActionType() == ActionType, false);

	return true;
}

// 다른 팀인 경우 true
bool UCDoAction::IsValidTeam(ACharacter* InOther)
{
	CheckNotValidResult(OwnerCharacter, false);

	IGenericTeamAgentInterface* iTeamAttacker = Cast<IGenericTeamAgentInterface>(OwnerCharacter.Get());
	IGenericTeamAgentInterface* iTeamOther = Cast<IGenericTeamAgentInterface>(InOther);

	if (iTeamAttacker != nullptr && iTeamOther != nullptr)
	{
		CheckTrueResult(iTeamAttacker->GetGenericTeamId() == iTeamOther->GetGenericTeamId(), false);
	}
	return true;
}
