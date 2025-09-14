#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CWeaponComponent.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "Actions/CAction_Action.h"
#include "Actions/CAction_Defend.h"
#include "Actions/CAction_Air.h"
#include "Actions/CAction_DownStrike.h"
#include "Actions/CAction_Airborne.h"
#include "Actions/CAction_Finish.h"
#include "Actions/CAction_Skill.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	// SubAttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
}

void UCWeaponAsset::BeginPlay(UCWeaponComponent* InOwnerWeapon, ACharacter* InOwnerCharacter, FWeaponData& InWeaponData)
{
	if (!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwnerCharacter;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		InWeaponData.Attachment = InOwnerCharacter->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);

		if (!!SubAttachmentClass)
		{
			InWeaponData.SubAttachment = InOwnerCharacter->GetWorld()->SpawnActor<ACAttachment>(SubAttachmentClass, params);
		}
	}

	if (!!EquipmentClass)
	{
		InWeaponData.Equipment = NewObject<UCEquipment>(InOwnerWeapon, EquipmentClass);
		CheckNull(InWeaponData.Equipment);

		InWeaponData.Equipment->BeginPlay(InOwnerCharacter, EquipmentData);

		if (!!InWeaponData.Attachment)
		{
			InWeaponData.Equipment->OnEquipmentBeginEquip.AddDynamic(InWeaponData.Attachment, &ACAttachment::OnEquipmentBeginEquip);
			InWeaponData.Equipment->OnEquipmentUnequip.AddDynamic(InWeaponData.Attachment, &ACAttachment::OnEquipmentUnequip);

			if (!!InWeaponData.SubAttachment)
			{
				InWeaponData.Equipment->OnEquipmentBeginEquip.AddDynamic(InWeaponData.SubAttachment, &ACAttachment::OnEquipmentBeginEquip);
				InWeaponData.Equipment->OnEquipmentUnequip.AddDynamic(InWeaponData.SubAttachment, &ACAttachment::OnEquipmentUnequip);
			}
		}
	}

	if (!!ActionClass)
	{
		InWeaponData.Action = NewObject<UCAction_Action>(InOwnerWeapon, ActionClass);
		CheckNull(InWeaponData.Action);

		InWeaponData.Action->BeginPlay(InOwnerCharacter, InWeaponData.Attachment, InWeaponData.Equipment, EActionType::Action);
	}

	if (!!DefendClass)
	{
		InWeaponData.Defend = NewObject<UCAction_Defend>(InOwnerWeapon, DefendClass);
		CheckNull(InWeaponData.Defend);

		InWeaponData.Defend->BeginPlay(InOwnerCharacter, InWeaponData.Attachment, InWeaponData.Equipment, EActionType::Defend);
	}

	if (!!Action_AirClass)
	{
		InWeaponData.Action_Air = NewObject<UCAction_Air>(InOwnerWeapon, Action_AirClass);
		CheckNull(InWeaponData.Action_Air);

		InWeaponData.Action_Air->BeginPlay(InOwnerCharacter, InWeaponData.Attachment, InWeaponData.Equipment, EActionType::Air);
	}

	if (!!Action_DownStrikeClass)
	{
		InWeaponData.Action_DownStrike = NewObject<UCAction_DownStrike>(InOwnerWeapon, Action_DownStrikeClass);
		CheckNull(InWeaponData.Action_DownStrike);

		InWeaponData.Action_DownStrike->BeginPlay(InOwnerCharacter, InWeaponData.Attachment, InWeaponData.Equipment, EActionType::DownStrike);
	}

	if (!!Action_AirborneClass)
	{
		InWeaponData.Action_Airborne = NewObject<UCAction_Airborne>(InOwnerWeapon, Action_AirborneClass);
		CheckNull(InWeaponData.Action_Airborne);

		InWeaponData.Action_Airborne->BeginPlay(InOwnerCharacter, InWeaponData.Attachment, InWeaponData.Equipment, EActionType::Airborne);
	}

	if (!!Action_FinishClass)
	{
		InWeaponData.Action_Finish = NewObject<UCAction_Finish>(InOwnerWeapon, Action_FinishClass);
		CheckNull(InWeaponData.Action_Finish);

		InWeaponData.Action_Finish->BeginPlay(InOwnerCharacter, InWeaponData.Attachment, InWeaponData.Equipment, EActionType::Finish);
	}

	if (!!Action_SkillClass)
	{
		InWeaponData.Action_Skill = NewObject<UCAction_Skill>(InOwnerWeapon, Action_SkillClass);
		CheckNull(InWeaponData.Action_Skill);

		InWeaponData.Action_Skill->BeginPlay(InOwnerCharacter, InWeaponData.Attachment, InWeaponData.Equipment, EActionType::Skill);
	}
}
