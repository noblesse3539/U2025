#include "BehaviorTree/CBTService_Combat.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CStateComponent.h"
#include "Characters/CPlayer.h"

UCBTService_Combat::UCBTService_Combat()
{
	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Combat::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{

	UBlackboardComponent* blackboard = SearchData.OwnerComp.GetBlackboardComponent();
	CheckNull(blackboard);

	EAIStateType aiState = (EAIStateType)blackboard->GetValueAsEnum(AIStateKey);
	CheckFalse(aiState == EAIStateType::Combat);
	// 랜덤으로 AI 상태값을 지정 ( Action, Defend, CircleMove )
	int32 num = UKismetMathLibrary::RandomIntegerInRange(0, 9);
	switch (num)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		{
			// 만약 타겟을 공격중인 적이 많을경우 CircleMove로
			ACPlayer* player = Cast<ACPlayer>(blackboard->GetValueAsObject(TargetKey));
			if (!!player)
			{
				if (player->IsPosessable())
				{
					player->Possess();
					blackboard->SetValueAsObject(PossessKey, player);
					blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Action);
				}
				else
				{
					blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::CircleMove);
				}
				break;
			}

			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Action);
			break;
		}
		case 7:
		case 8:
		{
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Defend);
			break;
		}
		case 9:
		{
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::CircleMove);
			break;
		}
	}
}