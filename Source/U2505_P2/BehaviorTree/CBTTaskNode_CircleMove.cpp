#include "BehaviorTree/CBTTaskNode_CircleMove.h"

#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

UCBTTaskNode_CircleMove::UCBTTaskNode_CircleMove()
{
	NodeName = "CircleMove";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_CircleMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	CheckNullResult(blackboard, EBTNodeResult::Failed);

	bool bClockwise = UKismetMathLibrary::RandomBool();
	blackboard->SetValueAsBool(ClockwiseKey, UKismetMathLibrary::RandomBool());

	float degree = abs(Degree);

	float radius = UKismetMathLibrary::RandomFloatInRange(RadiusRange.X, RadiusRange.Y);
	blackboard->SetValueAsFloat(RadiusKey, radius);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_CircleMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	CheckNull(blackboard);

	ACharacter* target = Cast<ACharacter>(blackboard->GetValueAsObject(TargetKey));

	CheckNull(target);

	FVector	 targetLocation = target->GetActorLocation();
	FVector	 aiLocation = ai->GetActorLocation();
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(targetLocation, aiLocation);

	bool bClockwise = blackboard->GetValueAsBool(ClockwiseKey);

	float degree = abs(Degree);
	if (bClockwise == false)
	{
		degree = -degree;
	}

	float	radius = blackboard->GetValueAsFloat(RadiusKey);
	FVector dest = targetLocation + (UKismetMathLibrary::CreateVectorFromYawPitch((rotator.Yaw + degree), 0.0f, 1.0f) * radius);

	controller->MoveToLocation(dest, 0.3f, false);

	if (bDebug)
	{
		UKismetSystemLibrary::DrawDebugLine(ai->GetWorld(), aiLocation, dest, FLinearColor::Yellow, 0.4f, 1.0f);
	}
}
