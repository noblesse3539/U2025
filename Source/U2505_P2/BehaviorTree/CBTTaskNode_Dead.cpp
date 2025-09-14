#include "BehaviorTree/CBTTaskNode_Dead.h"
#include "Global.h"
#include "Characters/CAIController.h"

EBTNodeResult::Type UCBTTaskNode_Dead::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	controller->ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::InProgress;
}
