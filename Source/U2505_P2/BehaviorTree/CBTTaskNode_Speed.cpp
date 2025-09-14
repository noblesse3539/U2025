#include "BehaviorTree/CBTTaskNode_Speed.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = StaticEnum<ESpeedType>()->GetNameStringByValue((int64)Type);
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(ai);
	CheckNullResult(movement, EBTNodeResult::Failed);

	switch (Type)
	{
		case ESpeedType::Walk:
			movement->OnWalk();
			break;
		case ESpeedType::Run:
			movement->OnRun();
			break;
		case ESpeedType::Sprint:
			movement->OnSprint();
			break;
	}

	return EBTNodeResult::Succeeded;
}

#if WITH_EDITOR
void UCBTTaskNode_Speed::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName name = PropertyChangedEvent.Property->GetFName();

	if (name == GET_MEMBER_NAME_CHECKED(UCBTTaskNode_Speed, Type))
		NodeName = StaticEnum<ESpeedType>()->GetNameStringByValue((int64)Type);
}
#endif