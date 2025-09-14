#include "BehaviorTree/CBTTaskNode_Possess.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CPlayer.h"

UCBTTaskNode_Possess::UCBTTaskNode_Possess()
{
	NodeName = "Possess";
}

EBTNodeResult::Type UCBTTaskNode_Possess::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	CheckNullResult(blackboard, EBTNodeResult::Failed);

	ACPlayer* player = Cast< ACPlayer>(blackboard->GetValueAsObject(PossessKey));
	if (!!player)
	{
		player->DisPossess();
		blackboard->SetValueAsObject(PossessKey, nullptr);
	}

	return EBTNodeResult::Failed;
}
