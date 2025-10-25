#include "Weapons/CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

void UCEquipment::BeginPlay(ACharacter* InOwnerCharacter, const FEquipmentData& InData)
{
	Data = InData;
	OwnerCharacter = InOwnerCharacter;

	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter.Get());
	Movement = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter.Get());
}

void UCEquipment::BeginDestroy()
{
	OnEquipmentBeginEquip.RemoveAll(this);
	OnEquipmentUnequip.RemoveAll(this);

	Super::BeginDestroy();
}

void UCEquipment::Equip_Implementation()
{
	CheckNotValid(OwnerCharacter);
	CheckNotValid(State);
	CheckNotValid(Movement);

	State->SetEquipMode();

	if (Data.bCanMove == false)
	{
		Movement->Stop();
	}

	if (!!Data.Montage)
	{
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);

		return;
	}

	// ��Ÿ�ְ� ���� ��쿡��(Fist ��) ������ �� �� �ֵ��� �Ʒ� �Լ��� ȣ��
	Begin_Equip();
	End_Equip();
}

void UCEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentBeginEquip.IsBound())
		OnEquipmentBeginEquip.Broadcast();
}

void UCEquipment::End_Equip_Implementation()
{
	CheckNotValid(State);
	CheckNotValid(Movement);

	if (Data.bCanMove == false)
	{
		Movement->Move();
	}
	State->SetIdleMode();
}

void UCEquipment::Unequip_Implementation()
{
	if (OnEquipmentUnequip.IsBound())
	{
		OnEquipmentUnequip.Broadcast();
	}
}
