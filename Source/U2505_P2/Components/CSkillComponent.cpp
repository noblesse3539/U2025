#include "Components/CSkillComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"

UCSkillComponent::UCSkillComponent()
{
	// PrimaryComponentTick.bCanEverTick = true;
}

void UCSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);
}

void UCSkillComponent::DoAction(int32 InSkillIdx)
{
	CheckTrue(InSkillIdx < 0);
	CheckFalse(Skills.Num() > InSkillIdx);

	TArray<FSkillData*> rows;
	Skills[InSkillIdx]->GetAllRows("", rows);
	FSkillData* skillData = rows[ComboIdx];
}

// void UCSkillComponent::DoAction(int32 InSkillIdx)
//{
//	CheckTrue(InSkillIdx < 0);
//	CheckFalse(Skills.Num() > InSkillIdx);
//
//	TArray<FSkillData*> rows;
//	Skills[InSkillIdx]->GetAllRows("", rows);
//	FSkillData* skillData = rows[ComboIdx];
//
//	// 몽타주
//	OwnerCharacter->PlayAnimMontage(skillData->Montage, skillData->PlayRate);
//
//	// Trace 세팅
//	FTransform transform = OwnerCharacter->GetActorTransform();
//	float	   z = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
//	float	   radius = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
//	FVector	   start = transform.GetLocation();
//	start += transform.GetRotation().GetForwardVector() * radius; // 전방 방향 시작지점 보정치
//	start.Z += skillData->OffsetZ;
//
//	FVector			end = start + transform.GetRotation().GetForwardVector() * skillData->Distance;
//	FVector			halfSize = skillData->HalfSize;
//	FRotator		orientation = FRotator(transform.GetRotation());
//	FName			profileName = "AttackSkill";
//	TArray<AActor*> ignore;
//	ignore.Add(OwnerCharacter);
//
//	EDrawDebugTrace::Type drawDebugType = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
//	TArray<FHitResult>	  hitResults;
//	FLinearColor		  traceColor = FLinearColor::Blue;
//	FLinearColor		  traceHitColor = FLinearColor::Red;
//	float				  DrawTime = 5.0f;
//
//	UKismetSystemLibrary::BoxTraceMultiByProfile(GetWorld(), start, end, halfSize, orientation, profileName, false, ignore, drawDebugType, hitResults, true, traceColor, traceHitColor, DrawTime);
//
//	ComboIdx = (ComboIdx + 1) % rows.Num();
//
//	CheckTrue(hitResults.Num() == 0);
//
//	for (FHitResult result : hitResults)
//	{
//		if (!!Cast<ACharacter>(result.GetActor()))
//		{
//			FLog::Print(result.GetActor()->GetName());
//		}
//	}
// }
