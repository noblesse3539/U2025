#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/CWeaponStructures.h"

#include "CWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS(ClassGroup = (Weapon), meta = (BlueprintSpawnableComponent))
class U2505_P2_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TMap<EWeaponType, class UCWeaponAsset*> DataAssetTable;

public:
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FORCEINLINE bool		IsUnarmedMode() { return CurrentType == EWeaponType::Max; }
	FORCEINLINE EActionType GetActionType() { return CurrentActionType; }
	FORCEINLINE EWeaponType GetWeaponType() { return CurrentType; }
	FORCEINLINE bool		IsEquipping() { return bEquipping == true; }

public:
	bool IsParry();

private:
	class UCWeaponAsset* GetWeaponAsset(EWeaponType InType);

public:
	class ACAttachment* GetAttachment(EWeaponType InType);
	class ACAttachment* GetAttachment();
	class UCEquipment*	GetEquipment(EWeaponType InType);
	class UCEquipment*	GetEquipment();
	class UCActionBase* GetAction(EWeaponType InType, EActionType InActionType);
	class UCActionBase* GetAction();

public:
	class UCAction_Action* GetAction_Action(EWeaponType InType);
	class UCAction_Action* GetAction_Action();

	class UCAction_Defend* GetAction_Defend(EWeaponType InType);
	class UCAction_Defend* GetAction_Defend();

	class UCAction_Air* GetAction_Air(EWeaponType InType);
	class UCAction_Air* GetAction_Air();

	class UCAction_DownStrike* GetAction_DownStrike(EWeaponType InType);
	class UCAction_DownStrike* GetAction_DownStrike();

	class UCAction_Airborne* GetAction_Airborne(EWeaponType InType);
	class UCAction_Airborne* GetAction_Airborne();

	class UCAction_Finish* GetAction_Finish(EWeaponType InType);
	class UCAction_Finish* GetAction_Finish();

	class UCAction_Skill* GetAction_Skill(EWeaponType InType);
	class UCAction_Skill* GetAction_Skill();

public:
	void Action_Action_Pressed();

	void Action_Defend_Pressed();
	void Action_Defend_Released();

	void Action_Air_Pressed();
	void Action_DownStrike_Pressed();
	void Action_Airborne_Pressed();
	void Action_Finish_Pressed();
	void Action_Skill_Pressed();

private:
	bool IsIdleMode();

public:
	void SetUnarmedMode();
	void SetFirstWeaponMode(); // DataAssetTable의 0번째 값으로 장착

	void SetFistMode();
	void SetSwordMode();
	void SetHammerMode();
	void SetAxeMode();
	void SetKatanaMode();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	void SetAction_Default();

public:
	void SetActionType(EActionType InType);

private:
	void ChangeActionType(EActionType InType);

	bool CanChangeActionType(EActionType InFromType, EActionType InToType);

public:
	void DoAction();
	void EndDoAction();
	void DoAction_Attack();

public:
	void Begin_Equip();
	void End_Equip();

public:
	void OnCharge();
	void OffCharge();

public:
	void Begin_DownStrike();

public:
	void AttachTo(FName InSocketName);

private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

public:
	FWeaponTypeChanged OnWeaponTypeChanged;
	FActionTypeChanged OnActionTypeChanged;

private:
	class ACharacter* OwnerCharacter;

private:
	EWeaponType CurrentType = EWeaponType::Max;

private:
	EActionType CurrentActionType = EActionType::Max;

private:
	UPROPERTY()
	TMap<EWeaponType, FWeaponData> WeaponDataMap;

private:
	float ChargingTime = 0.0f;
	float ChargeThreshold = 0.7f;
	bool  bCharge;

	bool bEquipping;
};
