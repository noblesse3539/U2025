#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.h"
#include "CDoAction.generated.h"

UCLASS(Abstract)
class U2505_P2_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginPlay(
		class ACharacter*			InOwnerCharacter,
		class ACAttachment*			InAttachment,
		class UCEquipment*			InEquipment,
		const TArray<FDoActionData> InDatas,
		const TArray<FDamagedData>	InDamagedDatas,
		const EActionType			InActionType);

	virtual void Tick(float InDeltaTime) {}

public:
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

public:
	FORCEINLINE TArray<FDoActionData> GetDatas() { return Datas; }
	FORCEINLINE TArray<FDamagedData> GetDamagedDatas() { return DamagedDatas; }

protected:
	class ACharacter* OwnerCharacter;
	class UWorld*	  World;

	TArray<FDoActionData> Datas;
	TArray<FDamagedData>  DamagedDatas;

protected:
	class UCStateComponent* State;

public:
	UFUNCTION()
	virtual void OnAttachmentBeginCollision() {}

	UFUNCTION()
	virtual void OnAttachmentEndCollision() {}

	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, UShapeComponent* InAttackCollision, class ACharacter* InOther) {}

	UFUNCTION()
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {}


// �Ļ����� ����� üũ �Լ���
protected:
	virtual bool IsValidActionType();
	virtual bool IsValidTeam(class ACharacter* InOther);

protected:
	EActionType ActionType;
};
