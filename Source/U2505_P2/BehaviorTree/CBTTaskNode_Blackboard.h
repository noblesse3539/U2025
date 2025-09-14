#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/CBTTaskNode.h"
#include "Components/CStateComponent.h"

#include "CBTTaskNode_Blackboard.generated.h"

UCLASS()
class U2505_P2_API UCBTTaskNode_Blackboard : public UCBTTaskNode
{
	GENERATED_BODY()

	UCBTTaskNode_Blackboard();

private:
	UPROPERTY(EditAnywhere)
	EAIStateType ToAIState = EAIStateType::None;

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FName AIStateKey = "AIState";

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
