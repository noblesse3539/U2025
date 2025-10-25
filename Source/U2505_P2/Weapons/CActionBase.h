#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CActionBase.generated.h"

class UCStateComponent;
class ACharacter;

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
	virtual void BeginDestroy() override;

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
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }

protected:
	UFUNCTION()
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) {}

protected:
	bool bInAction;

	TWeakObjectPtr<ACharacter> OwnerCharacter;

	class ACAttachment* Attachment;

	UPROPERTY()
	class UCDoAction* DoAction;

	TWeakObjectPtr<UCStateComponent> State;
};
