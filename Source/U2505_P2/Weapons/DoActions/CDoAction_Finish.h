#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Finish.generated.h"

UCLASS(Blueprintable)
class U2505_P2_API UCDoAction_Finish : public UCDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	bool bDebug;

	UPROPERTY(EditAnywhere)
	float Dist = 0.0f;

	UPROPERTY(EditAnywhere)
	float Radius = 400.0f;

	UPROPERTY(EditAnywhere)
	float Degree = 45.0f;

	UPROPERTY(EditAnywhere)
	float ActionDist = 140.0f;

public:
	void Tick(float InDeltaTime) override;

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	void FindTarget();
	void MoveToTarget(float InDeltaTime);

private:
	bool  ValidTarget(class ACharacter* InTarget);
	float CalcDistance(class ACharacter* InTarget);

private:
	ACharacter* Target;
};
