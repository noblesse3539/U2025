#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Weapons/CWeaponStructures.h"
#include "Weapons/Actions/CAction_Defend.h"
#include "Characters/CPlayer.h"

void UCDoAction_Combo::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	CheckNotValid(OwnerCharacter);
	CheckNotValid(Candidate);

	APlayerController* controller = OwnerCharacter.Get()->GetController<APlayerController>();
	CheckNull(controller);

	FRotator ownerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter.Get()->GetActorLocation(), Candidate->GetActorLocation());
	FRotator controlRotation = OwnerCharacter.Get()->GetControlRotation();

	ownerToTarget.Pitch = controlRotation.Pitch;

	if (controlRotation.Equals(ownerToTarget, FinishHittedAngle))
	{
		Candidate = nullptr;
		controller->SetControlRotation(ownerToTarget);

		return;
	}

	ownerToTarget.Roll = controlRotation.Roll;
	FRotator result = UKismetMathLibrary::RInterpTo(controlRotation, ownerToTarget, InDeltaTime, RotationSpeed);

	controller->SetControlRotation(result);
}

void UCDoAction_Combo::DoAction()
{
	CheckNotValid(OwnerCharacter);
	CheckNotValid(State);

	if (bEnable)
	{
		bEnable = false;
		bExist = true;
		return;
	}

	CheckFalse(Datas.Num() > 0);

	CheckFalse(State.Get()->IsIdleMode());

	Super::DoAction();
	Datas[Index].DoAction(OwnerCharacter.Get());
}

void UCDoAction_Combo::Begin_DoAction()
{
	CheckNotValid(OwnerCharacter);
	// 적 AI의 콤보 확률 설정
	if (!OwnerCharacter->IsA<ACPlayer>())
	{
		float temp = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
		FLog::Log(temp);
		if (temp >= 1.0f - ComboRate)
		{
			bExist = true;
		}
	}

	CheckFalse(bExist);

	bExist = false;

	Super::Begin_DoAction();

	CheckFalse((int32)(Index + 1) < Datas.Num());
	Index++;

	Datas[Index].DoAction(OwnerCharacter.Get());
}

void UCDoAction_Combo::End_DoAction()
{
	CheckNotValid(OwnerCharacter);

	Super::End_DoAction();

	Datas[Index].End_DoAction(OwnerCharacter.Get());

	Index = 0;
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	CheckNotValid(OwnerCharacter);

	Super::OnAttachmentEndCollision();

	if (Datas[Index].bFixedCamera == false)
	{
		Hits.Empty();

		return;
	}

	float	angle = FLT_MIN;
	FVector location = OwnerCharacter.Get()->GetActorLocation();

	for (ACharacter* character : Hits)
	{
		if (!IsValid(character))
			continue;

		FVector direction = character->GetActorLocation() - location;
		direction = direction.GetUnsafeNormal2D();

		FVector forward = OwnerCharacter.Get()->GetActorForwardVector();
		float	dot = FVector::DotProduct(direction, forward);

		if (dot < HittedAngle || dot < angle)
			continue;

		angle = dot;
		Candidate = character;
		break;
	}

	Hits.Empty();
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, UShapeComponent* InAttackCollision, ACharacter* InOther)
{
	CheckNull(OwnerCharacter);
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

	bool bFirstHit = Hits.Num() == 0;

	Hits.Add(InOther);

	CheckFalse(Index < (uint32)DamagedDatas.Num());
	DamagedDatas[Index].SendDamage(OwnerCharacter.Get(), InAttackCauser, InAttackCollision, InOther, bFirstHit);
}
