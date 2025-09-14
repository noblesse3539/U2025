#include "BehaviorTree/CBTTask_MoveTo.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

UCBTTask_MoveTo::UCBTTask_MoveTo()
{
	NodeName = "CMoveTo";

	bNotifyTick = true;
}

void UCBTTask_MoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(ai);
	CheckNull(movement);

	CheckFalse(movement->GetCanMove());

	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
