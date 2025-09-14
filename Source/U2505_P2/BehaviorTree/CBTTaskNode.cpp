#include "BehaviorTree/CBTTaskNode.h"
#include "Global.h"

UCBTTaskNode::UCBTTaskNode()
{
	bDebug = FHelpers::GetDebug();
}

EBTNodeResult::Type UCBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (bDebug)
	{
		FString s = OwnerComp.GetOwner()->GetName() + " - ";
		s += "ExecuteTask : " + NodeName;

		FLog::Log(s);
	}
	return EBTNodeResult::Type();
}

EBTNodeResult::Type UCBTTaskNode::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	if (bDebug)
	{
		FString s = OwnerComp.GetOwner()->GetName() + " - ";
		s += "AbortTask : " + NodeName;

		FLog::Log(s);
	}
	return EBTNodeResult::Type();
}
