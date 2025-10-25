#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

UCLASS(Blueprintable)
class U2505_P2_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Hitted")
	float HittedAngle = 0.20f;

	UPROPERTY(EditDefaultsOnly, Category = "Hitted")
	float FinishHittedAngle = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Hitted")
	float RotationSpeed = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Auto")
	float ComboRate = 0.7f; // Àû AIÀÇ ÄÞº¸ È®·ü

public:
	void Tick(float InDeltaTime) override;

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	FORCEINLINE void OnEnableCombo() { bEnable = true; }
	FORCEINLINE void OffEnableCombo() { bEnable = false; }

protected:
	UINT Index;

private:
	bool bEnable;
	bool bExist;

private:
	UPROPERTY()
	TArray<class ACharacter*> Hits;

	TWeakObjectPtr<ACharacter> Candidate;

public:
	void OnAttachmentEndCollision() override;
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class UShapeComponent* InAttackCollision, class ACharacter* InOther) override;
};
