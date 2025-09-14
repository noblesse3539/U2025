#include "BehaviorTree/CBTTaskNode_Blackboard.h"
#include "Global.h"

#include "BehaviorTree/BlackboardComponent.h"

UCBTTaskNode_Blackboard::UCBTTaskNode_Blackboard()
{
	NodeName = "AIState : " + StaticEnum<EAIStateType>()->GetNameStringByValue((int64)ToAIState);
}

EBTNodeResult::Type UCBTTaskNode_Blackboard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	CheckTrueResult(ToAIState == EAIStateType::None, EBTNodeResult::Failed);

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	CheckNullResult(blackboard, EBTNodeResult::Failed);

	blackboard->SetValueAsEnum(AIStateKey, (uint8)ToAIState);

	return EBTNodeResult::Succeeded;
}

#if WITH_EDITOR
void UCBTTaskNode_Blackboard::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName name = PropertyChangedEvent.Property->GetFName();

	if (name == GET_MEMBER_NAME_CHECKED(UCBTTaskNode_Blackboard, ToAIState))
		NodeName = "AIState : " + StaticEnum<EAIStateType>()->GetNameStringByValue((int64)ToAIState);
}
#endif
