#include "BehaviorTree/CBTTaskNode_Knockdown.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CAIController.h"

#include "Components/CStateComponent.h"

UCBTTaskNode_Knockdown::UCBTTaskNode_Knockdown()
{
	NodeName = "Knockdown";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Knockdown::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);
	
	controller->ClearFocus(EAIFocusPriority::Gameplay);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Knockdown::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	EAIStateType		  type = (EAIStateType)blackboard->GetValueAsEnum("AIState");

	if (type != EAIStateType::Knockdown)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}