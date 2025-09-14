#include "Weapons/CActionBase.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
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

	State = FHelpers::GetComponent<UCStateComponent>(InOwnerCharacter);

	State->OnStateTypeChanged.AddDynamic(this, &UCActionBase::OnStateTypeChanged);

	Movement = FHelpers::GetComponent<UCMovementComponent>(InOwnerCharacter);

	if (!!DoActionClass)
	{
		DoAction = NewObject<UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(InOwnerCharacter, InAttachment, InEquipment, DoActionDatas, DamagedDatas, InActionType);

		if (!!Attachment)
		{
			Attachment->OnAttachmentBeginCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginCollision);
			Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);

			Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
			Attachment->OnAttachmentEndOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndOverlap);

			UCWeaponTraceComponent* weaponTrace = FHelpers::GetComponent<UCWeaponTraceComponent>(Attachment);
			if (!!weaponTrace)
			{
				weaponTrace->OnWeaponTraceBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
			}
		}

		// if (!!Equipment) {
		//	InWeaponData.Equipment->OnEquipmentBeginEquip.AddDynamic(InWeaponData.Action, &UCDoAction::OnEquipmentBeginEquip);
		//	InWeaponData.Equipment->OnEquipmentEndEquip.AddDynamic(InWeaponData.Action, &UCDoAction::OnEquipmentEndEquip);
		//	InWeaponData.Equipment->OnEquipmentUnequip.AddDynamic(InWeaponData.Action, &UCDoAction::OnEquipmentUnequip);
		// }
	}
}

void UCActionBase::Pressed()
{
	bInAction = true;
}

void UCActionBase::Released()
{
	bInAction = false;
}
