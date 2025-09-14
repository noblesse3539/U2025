#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/CBTTaskNode.h"
#include "CBTTaskNode_Approach.generated.h"

UCLASS()
class U2505_P2_API UCBTTaskNode_Approach : public UCBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FName TargetKey = "Target";

public:
	UCBTTaskNode_Approach();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
