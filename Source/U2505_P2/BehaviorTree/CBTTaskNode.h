#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode.generated.h"

UCLASS()
class U2505_P2_API UCBTTaskNode : public UBTTaskNode
{
protected:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebug;

	GENERATED_BODY()

public:
	UCBTTaskNode();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
