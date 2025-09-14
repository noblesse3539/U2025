#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponStructures.h"

#include "CWeaponAsset.generated.h"

UCLASS()
class U2505_P2_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType = EWeaponType::Max;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> SubAttachmentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAction_Action> ActionClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAction_Defend> DefendClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAction_Air> Action_AirClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAction_DownStrike> Action_DownStrikeClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAction_Airborne> Action_AirborneClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAction_Finish> Action_FinishClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCAction_Skill> Action_SkillClass;

	UPROPERTY(EditAnywhere)
	FEquipmentData EquipmentData;

public:
	UCWeaponAsset();

private:
	void BeginPlay(class UCWeaponComponent* InOwnerWeapon, class ACharacter* InOwnerCharacter, FWeaponData& InWeaponData);

private:
	friend class UCWeaponComponent;
};
