#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/CBTTaskNode.h"
#include "CBTTaskNode_Possess.generated.h"

UCLASS()
class U2505_P2_API UCBTTaskNode_Possess : public UCBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere)
	FName PossessKey = "Possess";

public:
	UCBTTaskNode_Possess();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
