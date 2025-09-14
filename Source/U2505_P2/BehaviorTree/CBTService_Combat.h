#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Combat.generated.h"

UCLASS()
class U2505_P2_API UCBTService_Combat : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FName AIStateKey = "AIState";

	UPROPERTY(EditAnywhere)
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere)
	FName PossessKey = "Possess";

public:
	UCBTService_Combat();

protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	// void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
