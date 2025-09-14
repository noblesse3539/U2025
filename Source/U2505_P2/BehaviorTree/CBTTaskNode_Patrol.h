#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/CBTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"

UCLASS()
class U2505_P2_API UCBTTaskNode_Patrol : public UCBTTaskNode
{
	GENERATED_BODY()

private:
	// UPROPERTY(EditAnywhere, Category = "Debug")
	// bool bDebug;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float AcceptanceRadius = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FName PatrolLocationKey = "PatrolLocation";

public:
	UCBTTaskNode_Patrol();

public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
