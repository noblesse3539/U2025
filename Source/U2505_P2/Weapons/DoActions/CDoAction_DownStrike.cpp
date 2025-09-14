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

	Super::DoAction();
	// �ε����� 0�϶� ���߿� �־����. ���ص� 500���� ����.

	if (Index == 0)
	{
		Datas[Index].DoAction(OwnerCharacter);

		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		OwnerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;

		UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
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

	Datas[Index].DoAction(OwnerCharacter);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	OwnerCharacter->LaunchCharacter(FVector(0, 0, -FallingPower), true, true);
}

void UCDoAction_DownStrike::End_DoAction()
{
	Super::End_DoAction();

	Datas[Index].End_DoAction(OwnerCharacter);

	Index = 0;

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
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
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InAttackCollision, InOther);

	CheckNull(OwnerCharacter);
	CheckTrue(InOther == OwnerCharacter);

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
			defend->ParringDamagedData.SendDamage(InOther, InAttackCauser, InAttackCollision, OwnerCharacter);
			return;
		}
	}

	// �ߺ��� ��� X
	CheckFalse(Hits.Find(InOther) == INDEX_NONE);

	bool bFirtHit = Hits.Num() == 0;

	Hits.Add(InOther);

	CheckFalse(DamagedDatas.Num() > 0); // ����� 3������ �ǰ��� �ϳ��� ó��
	DamagedDatas[0].SendDamage(OwnerCharacter, InAttackCauser, InAttackCollision, InOther, bFirtHit);
}
