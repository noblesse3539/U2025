#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "Squads/ISquadable.h"
#include "GenericTeamAgentInterface.h"

#include "CEnemy_AI.generated.h"

UCLASS()
class U2505_P2_API ACEnemy_AI : public ACEnemy, public IISquadable, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Team")
	FGenericTeamId TeamID = 2;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

public:
	ACEnemy_AI();

public:
	void End_Dead() override;

protected:
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

public:
	class ACSquad* GetOwnerSquad() override;
	void		   SetOwnerSquad(class ACSquad* InOwnerSquad, uint8 InTeamId) override;
	uint8		   GetSquadTeamId() override;
	void		   SetOrderTarget(class ACharacter* InTarget) override;

public:
	void SetBodyColor(FLinearColor InColor);

public:
	void		   SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	FGenericTeamId GetGenericTeamId() const override;

private:
	class ACSquad* OwnerSquad = nullptr;
	FName		   TargetKey = "Target";
	FName		   OrderTargetKey = "OrderTarget";
	FName		   AIStateKey = "AIState";
};
