#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CActionBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class U2505_P2_API UCActionBase : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Action")
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FDamagedData> DamagedDatas;

public:
	UCActionBase();

public:
	virtual void BeginPlay(class ACharacter* InOwnerCharacter, class ACAttachment* InAttachment, class UCEquipment* InEquipment, const EActionType InActionType);
	virtual void Tick(float InDeltaTime) {}

public:
	virtual void Pressed();
	virtual void Released();

public:
	UFUNCTION(BlueprintNativeEvent)
	void		 Begin_Action();
	virtual void Begin_Action_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void		 End_Action();
	virtual void End_Action_Implementation() {}

public:
	FORCEINLINE UCDoAction* GetDoAction() { return DoAction; }

protected:
	UFUNCTION()
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) {}

protected:
	bool bInAction;

	class ACharacter* OwnerCharacter;

	class ACAttachment* Attachment;

	UPROPERTY()
	class UCDoAction* DoAction;

	class UCStateComponent*	   State;
	class UCMovementComponent* Movement;
};
