#include "BehaviorTree/CBTTaskNode_Defend.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Defend::UCBTTaskNode_Defend()
{
	NodeName = "Defend";

}

EBTNodeResult::Type UCBTTaskNode_Defend::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);
	
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);
	CheckNullResult(state, EBTNodeResult::Failed);

	if (!state->IsDefendMode()) {
		weapon->Action_Defend_Pressed();
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCBTTaskNode_Defend::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);
	CheckNullResult(state, EBTNodeResult::Failed);

	if (state->IsDefendMode()) {
		weapon->Action_Defend_Released();
	}

	return EBTNodeResult::Succeeded;
}
