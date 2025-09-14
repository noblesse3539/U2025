#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Weapons/CWeaponStructures.h"
#include "Weapons/Actions/CAction_Defend.h"

void UCDoAction_Combo::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	CheckNull(Candidate);

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(controller);

	FRotator ownerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Candidate->GetActorLocation());
	FRotator controlRotation = OwnerCharacter->GetControlRotation();

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
	if (bEnable)
	{
		bEnable = false;
		bExist = true;
		return;
	}

	CheckFalse(Datas.Num() > 0);

	CheckFalse(State->IsIdleMode());

	Super::DoAction();
	Datas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::Begin_DoAction()
{
	CheckFalse(bExist);

	bExist = false;

	Super::Begin_DoAction();

	CheckFalse((int32)(Index + 1) < Datas.Num());
	Index++;

	Datas[Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	Datas[Index].End_DoAction(OwnerCharacter);

	Index = 0;
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();

	if (Datas[Index].bFixedCamera == false)
	{
		Hits.Empty();

		return;
	}

	float	angle = FLT_MIN;
	FVector location = OwnerCharacter->GetActorLocation();

	for (ACharacter* character : Hits)
	{
		if (!IsValid(character))
			continue;

		FVector direction = character->GetActorLocation() - location;
		direction = direction.GetUnsafeNormal2D();

		FVector forward = OwnerCharacter->GetActorForwardVector();
		float	dot = FVector::DotProduct(direction, forward);

		if (dot < HittedAngle || dot < angle)
			continue;

		angle = dot;
		Candidate = character;
	}

	Hits.Empty();
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, UShapeComponent* InAttackCollision, ACharacter* InOther)
{
	
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InAttackCollision, InOther);
	
	CheckNull(OwnerCharacter);
	CheckTrue(InOther == OwnerCharacter);

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
			defend->ParringDamagedData.SendDamage(InOther, InAttackCauser, InAttackCollision, OwnerCharacter);
			return;
		}
	}

	// 중복일 경우 X
	CheckFalse(Hits.Find(InOther) == INDEX_NONE);

	bool bFirstHit = Hits.Num() == 0;

	Hits.Add(InOther);

	CheckFalse(Index < (uint32)DamagedDatas.Num());
	DamagedDatas[Index].SendDamage(OwnerCharacter, InAttackCauser, InAttackCollision, InOther, bFirstHit);
}
