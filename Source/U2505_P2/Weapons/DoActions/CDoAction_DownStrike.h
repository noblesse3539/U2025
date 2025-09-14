#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_DownStrike.generated.h"

UCLASS(Blueprintable)
class U2505_P2_API UCDoAction_DownStrike : public UCDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float FallingPower = 3000.0f;

public:
	void Tick(float InDeltaTime) override;

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

private:
	UINT Index;

private:
	UPROPERTY()
	TArray<class ACharacter*> Hits;

public:
	void OnAttachmentEndCollision() override;
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class UShapeComponent* InAttackCollision, class ACharacter* InOther) override;

private:
	FName Socket_Origin = "MainHandSocket";
	FName Socket_Action = "MainHandSocket_Reverse";
};
