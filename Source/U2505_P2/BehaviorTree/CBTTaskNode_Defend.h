#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/CBTTaskNode.h"
#include "CBTTaskNode_Defend.generated.h"

UCLASS()
class U2505_P2_API UCBTTaskNode_Defend : public UCBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Defend();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	// void				TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
