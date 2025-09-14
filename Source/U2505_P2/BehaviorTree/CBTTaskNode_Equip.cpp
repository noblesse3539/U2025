#include "BehaviorTree/CBTTaskNode_Equip.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"

#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);
	CheckNullResult(state, EBTNodeResult::Failed);

	controller->StopMovement();

	if (Type == ETaskEquipType::Equip)
	{
		CheckFalseResult(weapon->IsUnarmedMode(), EBTNodeResult::Failed);
	}
	else if (Type == ETaskEquipType::Unequip)
	{
		CheckTrueResult(weapon->IsUnarmedMode(), EBTNodeResult::Succeeded);
	}

	weapon->SetFirstWeaponMode();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);
	CheckNull(state);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNull(weapon);

	CheckTrue(weapon->IsEquipping());

	if (Type == ETaskEquipType::Equip)
	{
		// ÀåÂø ¿Ï·á°¡ µÆ´Ù¸é Succeeded
		if (!weapon->IsUnarmedMode())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else if (Type == ETaskEquipType::Unequip)
	{
		if (weapon->IsUnarmedMode())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNullResult(weapon, EBTNodeResult::Failed);

	weapon->End_Equip();

	return EBTNodeResult::Aborted;
}

#if WITH_EDITOR
void UCBTTaskNode_Equip::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName name = PropertyChangedEvent.Property->GetFName();

	if (name == GET_MEMBER_NAME_CHECKED(UCBTTaskNode_Equip, Type))
		NodeName = StaticEnum<ETaskEquipType>()->GetNameStringByValue((int64)Type);
}
#endif
