#include "Characters/CEnemy_AI.h"
#include "Global.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Squads/CSquad.h"

#include "Components/CStateComponent.h"
#include "CAIController.h"

ACEnemy_AI::ACEnemy_AI()
	: Super()
{
}

void ACEnemy_AI::End_Dead()
{
	if (IsValid(OwnerSquad))
	{
		OwnerSquad->Members.Remove(this);
	}
	Super::End_Dead();
}

void ACEnemy_AI::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	ACAIController*		  controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();

	switch (InNewType)
	{
		case EStateType::Idle:
		case EStateType::Max:
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Max);
			break;
		case EStateType::Damaged:
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Damaged);
			break;
		case EStateType::Knockdown:
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Knockdown);
			break;
		case EStateType::Dead:
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Dead);
			break;
	}

	Super::OnStateTypeChanged(InPrevType, InNewType);
}

ACSquad* ACEnemy_AI::GetOwnerSquad()
{
	return OwnerSquad;
}

void ACEnemy_AI::SetOwnerSquad(ACSquad* InOwnerSquad, uint8 InTeamId)
{
	OwnerSquad = InOwnerSquad;
	SetGenericTeamId(FGenericTeamId(InTeamId));

	AController* controllerTemp = GetController();
	CheckNull(controllerTemp);

	ACAIController* controller = Cast<ACAIController>(controllerTemp);
	CheckNull(controller);

	controller->SetGenericTeamId(InTeamId);
}

uint8 ACEnemy_AI::GetSquadTeamId()
{
	return (uint8)TeamID;
}

void ACEnemy_AI::SetOrderTarget(ACharacter* InTarget)
{
	AController* controllerTemp = GetController();
	CheckNull(controllerTemp);

	ACAIController* controller = Cast<ACAIController>(controllerTemp);
	CheckNull(controller);

	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	CheckNull(blackboard);

	// 이미 싸우는 타겟 존재시 리턴
	ACharacter* target = Cast<ACharacter>(blackboard->GetValueAsObject(TargetKey));
	CheckTrue(IsValid(target));

	blackboard->SetValueAsObject(OrderTargetKey, InTarget);
}

void ACEnemy_AI::SetBodyColor(FLinearColor InColor)
{
	TArray<UMaterialInterface*> materials = GetMesh()->GetMaterials();

	for (int32 i = 0; i < materials.Num(); i++)
	{
		UMaterialInstanceDynamic* DynMaterial = GetMesh()->CreateDynamicMaterialInstance(i, materials[i]);

		DynMaterial->SetVectorParameterValue(FName("BaseColorMul"), InColor);
	}
}

void ACEnemy_AI::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	TeamID = InTeamID;
}

FGenericTeamId ACEnemy_AI::GetGenericTeamId() const
{
	return TeamID;
}
