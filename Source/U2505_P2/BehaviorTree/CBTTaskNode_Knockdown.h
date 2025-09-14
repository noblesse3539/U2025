#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/CBTTaskNode.h"
#include "CBTTaskNode_Knockdown.generated.h"

UCLASS()
class U2505_P2_API UCBTTaskNode_Knockdown : public UCBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Knockdown();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void				TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
