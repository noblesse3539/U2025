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
	// 인덱스가 0일땐 공중에 있어야함. 못해도 500으로 잡자.

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

	// ActionType 검사
	CheckFalse(IsValidActionType());

	// 같은 팀인지 확인
	CheckFalse(IsValidTeam(InOther));

	// Other 패링시
	UCWeaponComponent* otherWeapon = FHelpers::GetComponent<UCWeaponComponent>(InOther);

	if (!!otherWeapon && otherWeapon->IsParry())
	{
		UCAction_Defend* defend = otherWeapon->GetAction_Defend();
		if (IsValid(defend))
		{
			// 패링 DoAction
			defend->ParringActionData.DoAction(InOther);

			// 패링 데미지 처리
			defend->ParringDamagedData.SendDamage(InOther, InAttackCauser, InAttackCollision, OwnerCharacter.Get());
			return;
		}
	}

	// 중복일 경우 X
	CheckFalse(Hits.Find(InOther) == INDEX_NONE);

	bool bFirtHit = Hits.Num() == 0;

	Hits.Add(InOther);

	CheckFalse(DamagedDatas.Num() > 0); // 모션은 3개지만 피격은 하나로 처리
	DamagedDatas[0].SendDamage(OwnerCharacter.Get(), InAttackCauser, InAttackCollision, InOther, bFirtHit);
}
