#include "BehaviorTree/CBTTaskNode_Approach.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTTaskNode_Approach::UCBTTaskNode_Approach()
{
	NodeName = "Approach";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Approach::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(ai);
	CheckNullResult(movement, EBTNodeResult::Failed);

	CheckFalseResult(movement->GetCanMove(), EBTNodeResult::Failed);

	return EBTNodeResult::Succeeded;
}
