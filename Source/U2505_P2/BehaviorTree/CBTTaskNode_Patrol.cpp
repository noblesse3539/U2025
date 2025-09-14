#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CEnemy_AI.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Characters/CAIController.h"



UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(OwnerComp.GetWorld());
	CheckNullResult(NavSys, EBTNodeResult::Failed);

	FNavLocation	 resultLocation;
	ANavigationData* UseNavData = NavSys->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);

	CheckFalseResult(NavSys->GetRandomPointInNavigableRadius(ai->StartLocation, Radius, resultLocation), EBTNodeResult::Failed);
	
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	blackboard->SetValueAsVector(PatrolLocationKey, resultLocation.Location);

	if (bDebug)
	{
		UKismetSystemLibrary::DrawDebugSphere(OwnerComp.GetWorld(), resultLocation.Location, 25.0f, 40, FLinearColor::Green, 5.0f);

		FVector start = ai->StartLocation;
		start.Z -= 25.0f;

		FVector end = ai->StartLocation;
		end.Z += 25.0f;

		UKismetSystemLibrary::DrawDebugCylinder(OwnerComp.GetWorld(), start, end, Radius, 40, FLinearColor::Blue, 5.0f);
	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	FVector location = blackboard->GetValueAsVector(PatrolLocationKey);

	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceRadius, false);

	if (result == EPathFollowingRequestResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
