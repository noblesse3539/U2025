#pragma once

#include "CoreMinimal.h"
#include "Weapons/CActionBase.h"
#include "Weapons/CWeaponStructures.h"
#include "CAction_Defend.generated.h"

UCLASS(Blueprintable)
class U2505_P2_API UCAction_Defend : public UCActionBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* DefendMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	float DefendMontage_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Parring")
	FDoActionData ParringActionData;

	UPROPERTY(EditAnywhere, Category = "Parring")
	FDamagedData ParringDamagedData;

public:
	UCAction_Defend();

public:
	void Pressed() override;
	void Released() override;

protected:
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

private:
	void OnDefend();
	void OffDefend(); // Defend�� �������� ���� ����� �ٷ� Default�� �������� �ʱ�����

	void ExpireParry();

public:
	FORCEINLINE bool IsParry() { return bParry; }

private:
	bool		 bParry;
	float		 ParryDuration = 0.5f;
	FTimerHandle Handle_Parry;
};
