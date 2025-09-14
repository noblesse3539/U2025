#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Characters/CStateStructures.h"

#include "CStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS()
class U2505_P2_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool	   IsIdleMode() { return Current == EStateType::Idle; }
	FORCEINLINE bool	   IsEquipMode() { return Current == EStateType::Equip; }
	FORCEINLINE bool	   IsActionMode() { return Current == EStateType::Action; }
	FORCEINLINE bool	   IsKnockdownMode() { return Current == EStateType::Knockdown; }
	FORCEINLINE bool	   IsDefendMode() { return Current == EStateType::Defend; }
	FORCEINLINE bool	   IsDeadMode() { return Current == EStateType::Dead; }
	FORCEINLINE EStateType GetStateType() const { return Current; }

public:
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetIdleMode();
	void SetEvadeMode();
	void SetEquipMode();
	void SetActionMode();
	void SetDamagedMode();
	void SetDeadMode();
	void SetKnockdownMode();
	void SetGetupMode();
	void SetDefendMode();

private:
	void ChangeType(EStateType InType);
	bool CanChangeType(EStateType InFromType, EStateType InToType);

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Current;
};
