#include "Weapons/DoActions/CDoAction_Finish.h"

#include "Global.h"
#include "GameFramework/Character.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeaponStructures.h"
#include "Weapons/CAttachment.h"
#include "Characters/CEnemy.h"

void UCDoAction_Finish::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	CheckNotValid(OwnerCharacter);
	CheckNull(Target);

	FVector direction = (Target->GetActorLocation() - OwnerCharacter.Get()->GetActorLocation()).GetSafeNormal();
	float	Distance = FVector::Dist(Target->GetActorLocation(), OwnerCharacter.Get()->GetActorLocation());
	if (Distance > ActionDist)
	{
		OwnerCharacter.Get()->AddMovementInput(direction, 1.0f);
	}
	else
	{
		DoAction();
		Target = nullptr;
	}
}

void UCDoAction_Finish::DoAction()
{
	CheckFalse(Datas.Num() > 0);
	CheckNotValid(OwnerCharacter);

	Super::DoAction();

	Datas[0].DoAction(OwnerCharacter.Get());

	CheckFalse(DamagedDatas.Num() > 0);
	CheckNull(Target);
	ACEnemy* enemy = Cast<ACEnemy>(Target);
	CheckNull(enemy);

	UCStateComponent* enemyState = FHelpers::GetComponent<UCStateComponent>(enemy);
	CheckNull(enemyState);

	enemy->SetDeadMontage(DamagedDatas[0].Montage, DamagedDatas[0].PlayRate);
	{
		FVector start = enemy->GetActorLocation();
		FVector look = OwnerCharacter.Get()->GetActorLocation();
		look.Z = start.Z;
		enemy->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, look));
	}
	{
		FVector start = OwnerCharacter.Get()->GetActorLocation();
		FVector look = enemy->GetActorLocation();
		look.Z = start.Z;
		OwnerCharacter.Get()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, look));
	}

	enemyState->SetDeadMode();
}

void UCDoAction_Finish::Begin_DoAction()
{
	Super::Begin_DoAction();
}

void UCDoAction_Finish::End_DoAction()
{
	CheckNotValid(OwnerCharacter);

	Super::End_DoAction();

	Datas[0].End_DoAction(OwnerCharacter.Get());
}

void UCDoAction_Finish::FindTarget()
{
	CheckNotValid(OwnerCharacter);

	// 트레이스로 전방 체력 30% 미만 적 탐색. 가장 가까운 적 리턴
	FVector			start = OwnerCharacter.Get()->GetActorLocation();
	FVector			end = OwnerCharacter.Get()->GetActorLocation() + OwnerCharacter.Get()->GetActorForwardVector() * Dist;
	TArray<AActor*> ignore;
	ignore.Add(OwnerCharacter.Get());

	EDrawDebugTrace::Type drawDebugType = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	TArray<FHitResult>	  hitResults;
	FLinearColor		  traceColor = FLinearColor::Blue;
	FLinearColor		  traceHitColor = FLinearColor::Red;
	float				  DrawTime = 5.0f;

	UKismetSystemLibrary::SphereTraceMultiByProfile(World, start, end, Radius, "AttackSkill", false, ignore, drawDebugType, hitResults, true, traceColor, traceHitColor, DrawTime);

	CheckTrue(hitResults.Num() == 0);

	for (const FHitResult& result : hitResults)
	{
		ACEnemy* current = Cast<ACEnemy>(result.GetActor());

		if (ValidTarget(current))
		{
			if (Target == nullptr)
			{
				Target = current;
			}
			else
			{
				float distTarget = CalcDistance(Target);
				float distCurrent = CalcDistance(current);
				if (distCurrent < distTarget)
				{
					Target = current;
				}
			}
		}
	}
}

void UCDoAction_Finish::MoveToTarget(float InDeltaTime)
{
}

// 유효성 검사 및 전방 각도 계산
bool UCDoAction_Finish::ValidTarget(ACharacter* InTarget)
{
	CheckNotValidResult(OwnerCharacter, false);
	CheckNullResult(InTarget, false);

	FVector toTarget = InTarget->GetActorLocation() - OwnerCharacter.Get()->GetActorLocation();
	toTarget.Z = 0.0f; // 높이는 무시함
	toTarget = toTarget.GetSafeNormal();
	float dot = FVector::DotProduct(OwnerCharacter.Get()->GetActorForwardVector(), toTarget);
	if (dot >= FMath::Cos(FMath::DegreesToRadians(Degree)))
		return true;

	return false;
}

float UCDoAction_Finish::CalcDistance(ACharacter* InTarget)
{
	CheckNotValidResult(OwnerCharacter, std::numeric_limits<float>::max());
	FVector toTarget = InTarget->GetActorLocation() - OwnerCharacter.Get()->GetActorLocation();

	float dist = FVector::Dist2D(InTarget->GetActorLocation(), OwnerCharacter.Get()->GetActorLocation());
	dist = FMath::Abs(dist);

	return dist;
}
