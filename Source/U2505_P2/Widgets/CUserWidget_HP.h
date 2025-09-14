#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_HP.generated.h"

UCLASS()
class U2505_P2_API UCUserWidget_HP : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxHealthPoint = 100;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float MaxStaminaPoint = 100;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float RecoverySpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float RecoveryInterpSpeed = 2.0f;

protected:
	void NativeOnInitialized() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	FORCEINLINE bool IsDead() { return HealthPoint <= 0.0f; }

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateHealthPoint();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateHealthTail();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateStaminaPoint();

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealthPointRatio();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealthTailRatio();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetStaminaPointRatio();

public:
	void Damage(float InAmount);
	void DecreaseStamina(float InAmount);

private:
	void TickHealthTail(float InDeltaTime);
	void TickStamina(float InDeltaTime);

private:
	float HealthPoint;
	float StaminaPoint;

	float HealthTail;

	bool bCanRecovery = true;
	bool bCanUpdateHealthTail = true;

	float Delay_RecoveryTimer = 3.5f;
	float Delay_TailTimer = 0.8f;

private:
	FTimerHandle Handle_healthTail;
	FTimerHandle Handle_Recovery;

	

public:
	FORCEINLINE void OnCanRecovery() { bCanRecovery = true; }
	FORCEINLINE void OffCanRecovery() { bCanRecovery = false; }

	FORCEINLINE void OnCanUpdateHealthTail() { bCanUpdateHealthTail = true; }
	FORCEINLINE void OffCanUpdateHealthTail() { bCanUpdateHealthTail = false; }

};
