#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnequip);

UCLASS()
class U2505_P2_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwnerCharacter, const FEquipmentData& InData);

public:
	UFUNCTION(BlueprintNativeEvent)
	void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Unequip();
	void Unequip_Implementation();

public:
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentUnequip	 OnEquipmentUnequip;

private:
	FEquipmentData Data;

private:
	class ACharacter*		   OwnerCharacter;
	class UCStateComponent*	   State;
	class UCMovementComponent* Movement;
};
