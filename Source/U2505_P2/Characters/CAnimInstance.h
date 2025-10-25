#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Weapons/CWeaponStructures.h"
#include "CFeetComponent.h"
#include "Components/CStateComponent.h"

#include "CAnimInstance.generated.h"

class UCWeaponComponent;
class UCStateComponent;
class UCFeetComponent;

UCLASS()
class U2505_P2_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bFalling;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bEquip_Sword;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bDefend;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	bool bKnockdown;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::Max;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Action")
	EActionType ActionType = EActionType::Max;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "State")
	EStateType StateType = EStateType::Max;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	bool bUseFootIK;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FFeetData FeetData;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Bow")
	bool bBowstring;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Bow")
	bool bPullBowstring;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Bow")
	FVectorData BowstringLocation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Bow")
	float Pitch;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
	// void OnMontageInstanceStopped(FAnimMontageInstance& StoppedMontageInstance) override;

public:
	virtual void BeginDestroy() override;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
	void OnActionTypeChanged(EActionType InPrevType, EActionType InNewType);

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;

private:
	TWeakObjectPtr<UCWeaponComponent> Weapon;
	TWeakObjectPtr<UCStateComponent>  State;

private:
	TWeakObjectPtr<UCFeetComponent> Feet;
};
