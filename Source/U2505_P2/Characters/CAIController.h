#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"

UCLASS()
class U2505_P2_API ACAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Perception;

public:
	ACAIController();

protected:
	void BeginPlay() override;

public:
	virtual void BeginDestroy() override;

protected:
	void OnPossess(APawn* InPawn) override;
	void OnUnPossess() override;

private:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	class ACEnemy_AI*			Enemy;
	class UAISenseConfig_Sight* Sight;
};
