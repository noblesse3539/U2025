#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_AI.generated.h"

UCLASS()
class U2505_P2_API UCBTService_AI : public UBTService
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere)
	FName OrderTargetKey = "OrderTarget";

	UPROPERTY(EditAnywhere)
	FName AIStateKey = "AIState";

	UPROPERTY(EditAnywhere)
	float ApproachDist = 200.0f;

public:
	UCBTService_AI();

protected:
	// void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class ACharacter* JudgeTarget(ACharacter* InTarget1, ACharacter* InTarget2);
};
