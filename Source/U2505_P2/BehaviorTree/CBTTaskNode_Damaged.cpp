#include "BehaviorTree/CBTTaskNode_Damaged.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Damaged::UCBTTaskNode_Damaged()
{
	NodeName = "Damaged";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Damaged::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	controller->StopMovement();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Damaged::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	EAIStateType		  type = (EAIStateType)blackboard->GetValueAsEnum("AIState");

	if (type != EAIStateType::Damaged)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}