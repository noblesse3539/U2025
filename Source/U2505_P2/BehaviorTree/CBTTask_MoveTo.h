#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "CBTTask_MoveTo.generated.h"

UCLASS()
class U2505_P2_API UCBTTask_MoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UCBTTask_MoveTo();

private:
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
