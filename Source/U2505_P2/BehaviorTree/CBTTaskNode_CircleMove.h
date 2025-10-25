#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/CBTTaskNode.h"
#include "CBTTaskNode_CircleMove.generated.h"

UCLASS()
class U2505_P2_API UCBTTaskNode_CircleMove : public UCBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere)
	FName ClockwiseKey = "bClockwise";

	UPROPERTY(EditAnywhere)
	FName RadiusKey = "Radius";

	UPROPERTY(EditAnywhere)
	float Degree = 20.0f;

	UPROPERTY(EditAnywhere)
	FVector2D RadiusRange = FVector2D(150, 500);

public:
	UCBTTaskNode_CircleMove();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void				TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	// EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
