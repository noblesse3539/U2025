#include "Weapons/DoActions/CDoAction_DownStrike.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "Weapons/CAttachment.h"
#include "Weapons/Actions/CAction_Defend.h"

void UCDoAction_DownStrike::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
}

void UCDoAction_DownStrike::DoAction()
{
	CheckFalse(Datas.Num() > 0);
	CheckNotValid(OwnerCharacter);

	Super::DoAction();
	// �ε����� 0�϶� ���߿� �־����. ���ص� 500���� ����.

	if (Index == 0)
	{
		Datas[Index].DoAction(OwnerCharacter.Get());

		OwnerCharacter.Get()->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		OwnerCharacter.Get()->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter.Get());
		CheckNull(weapon);

		ACAttachment* attachment = weapon->GetAttachment();
		CheckNull(attachment);

		attachment->AttachTo(Socket_Action);
	}
}

void UCDoAction_DownStrike::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse((int32)(Index + 1) < Datas.Num());
	Index++;

	Datas[Index].DoAction(OwnerCharacter.Get());

	OwnerCharacter.Get()->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	OwnerCharacter.Get()->LaunchCharacter(FVector(0, 0, -FallingPower), true, true);
}

void UCDoAction_DownStrike::End_DoAction()
{
	CheckNotValid(OwnerCharacter);

	Super::End_DoAction();

	Datas[Index].End_DoAction(OwnerCharacter.Get());

	Index = 0;

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter.Get());
	CheckNull(weapon);

	ACAttachment* attachment = weapon->GetAttachment();
	CheckNull(attachment);

	attachment->AttachTo(Socket_Origin);
}

void UCDoAction_DownStrike::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();
	Hits.Empty();
}

void UCDoAction_DownStrike::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, UShapeComponent* InAttackCollision, ACharacter* InOther)
{
	CheckNotValid(OwnerCharacter);
	CheckTrue(InOther == OwnerCharacter);

	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InAttackCollision, InOther);

	// ActionType �˻�
	CheckFalse(IsValidActionType());

	// ���� ������ Ȯ��
	CheckFalse(IsValidTeam(InOther));

	// Other �и���
	UCWeaponComponent* otherWeapon = FHelpers::GetComponent<UCWeaponComponent>(InOther);

	if (!!otherWeapon && otherWeapon->IsParry())
	{
		UCAction_Defend* defend = otherWeapon->GetAction_Defend();
		if (IsValid(defend))
		{
			// �и� DoAction
			defend->ParringActionData.DoAction(InOther);

			// �и� ������ ó��
			defend->ParringDamagedData.SendDamage(InOther, InAttackCauser, InAttackCollision, OwnerCharacter.Get());
			return;
		}
	}

	// �ߺ��� ��� X
	CheckFalse(Hits.Find(InOther) == INDEX_NONE);

	bool bFirtHit = Hits.Num() == 0;

	Hits.Add(InOther);

	CheckFalse(DamagedDatas.Num() > 0); // ����� 3������ �ǰ��� �ϳ��� ó��
	DamagedDatas[0].SendDamage(OwnerCharacter.Get(), InAttackCauser, InAttackCollision, InOther, bFirtHit);
}
