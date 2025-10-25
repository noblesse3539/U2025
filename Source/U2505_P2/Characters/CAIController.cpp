#include "Characters/CAIController.h"

#include "Global.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Squads/CSquad.h"
#include "Characters/CEnemy_AI.h"

ACAIController::ACAIController()
{
	FHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	FHelpers::CreateActorComponent<UAISenseConfig_Sight>(this, &Sight, "Sight");

	Sight->SightRadius = 500.0f;
	Sight->LoseSightRadius = 600.0f;
	Sight->PeripheralVisionAngleDegrees = 180.0f;
	Sight->SetMaxAge(4.0f);
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation()); // 감각의 최우선순위 지정.
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::BeginDestroy()
{
	if (IsValid(Perception))
		Perception->OnPerceptionUpdated.RemoveDynamic(this, &ACAIController::OnPerceptionUpdated);

	Super::BeginDestroy();
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);
	CheckNull(Enemy);

	SetGenericTeamId(Enemy->GetGenericTeamId());

	CheckNull(Enemy->GetBehaviorTree());
	RunBehaviorTree(Enemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	CheckNull(Blackboard);

	if (actors.IsEmpty() == false)
	{
		ACharacter* target = Cast<ACharacter>(actors[0]);
		ACSquad*	squad = Enemy->GetOwnerSquad();

		if (IsValid(squad))
		{
			squad->OnOrder(target);
		}

		Blackboard->SetValueAsObject("Target", target);
	}
	else
	{
		Blackboard->SetValueAsObject("Target", nullptr);
	}
}
