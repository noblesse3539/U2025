#include "BehaviorTree/CBTService_AI.h"
#include "Global.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

UCBTService_AI::UCBTService_AI()
{
	NodeName = "CBTService_AI";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_AI::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	CheckNull(blackboard);

	EAIStateType aiState = (EAIStateType)blackboard->GetValueAsEnum(AIStateKey);

	// 아래 상태들은 서비스 진입시 튕김
	CheckTrue(aiState == EAIStateType::CircleMove);
	CheckTrue(aiState == EAIStateType::Damaged);
	CheckTrue(aiState == EAIStateType::Knockdown);
	CheckTrue(aiState == EAIStateType::Equip);
	CheckTrue(aiState == EAIStateType::Dead);

	ACharacter* target = Cast<ACharacter>(blackboard->GetValueAsObject(TargetKey));
	ACharacter* ordertarget = Cast<ACharacter>(blackboard->GetValueAsObject(OrderTargetKey));

	target = JudgeTarget(ordertarget, target);
	blackboard->SetValueAsObject(TargetKey, target);

	// 상태 - 적 순찰 (Patrol)
	if (target == nullptr)
	{
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Patrol);

		return;
	}

	// 타겟 포커스
	controller->SetFocus(target, EAIFocusPriority::Gameplay);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(ai);
	CheckNull(weapon);

	// 상태 - 무기 장착 (Equip)
	if (weapon->IsUnarmedMode())
	{
		blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Equip); // weapon->SetFirstWeaponMode();
		return;
	}

	// 상태 - 적 전투 (Combat)
	float dist = ai->GetHorizontalDistanceTo(target);
	if (dist <= ApproachDist)
	{
		CheckTrue(aiState == EAIStateType::Action);
		CheckTrue(aiState == EAIStateType::Defend);
		CheckTrue(aiState == EAIStateType::CircleMove);
		blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Combat);

		return;
	}

	// 상태 - 적 접근 (Approach)
	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(ai);
	CheckNull(movement);
	if (movement->GetCanMove())
	{
		blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Approach);
	}
}

ACharacter* UCBTService_AI::JudgeTarget(ACharacter* InTarget1, ACharacter* InTarget2)
{
	bool check1 = IsValid(InTarget1);
	bool check2 = IsValid(InTarget2);

	// 둘 다 유효하지 않을 경우 nullptr
	if (!check1 && !check2)
	{
		return nullptr;
	}

	// 하나만 유효할 경우
	if (!(check1 && check2))
	{
		if (check1)
			return InTarget1;

		return InTarget2;
	}

	// 둘 다 유효할 경우 InTarget1을 리턴
	return InTarget1;
}
