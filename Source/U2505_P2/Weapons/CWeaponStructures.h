#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DamageEvents.h"
#include "CWeaponStructures.generated.h"

UENUM(BlueprintType)
enum class EDamagedStateType : uint8
{
	Normal = 0,
	Flying,		 // 공중으로 띄움
	GroundPound, // 땅으로 내려침
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Action = 0, // 일반콤보
	Air,		// 공중콤보
	DownStrike, // 공중에서 공격
	Airborne,	// 띄우기공격
	Finish,		// 피니시
	Defend,		// 방어시 (에셋에는 패링 시 애니메이션 등 저장)
	Skill,		// 스킬 사용
	Max,
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fist = 0,
	Sword,
	Hammer,
	Axe,
	Katana,
	Magic,
	Bow,
	Max,
};

USTRUCT(BlueprintType)
struct FVectorData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bChecked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector V = FVector::ZeroVector;

public:
	operator FVector() { return V; }
};

USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;
};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bFixedCamera = false;

	UPROPERTY(EditAnywhere)
	bool bUseRotation = true;

	UPROPERTY(EditAnywhere)
	float Cost = 0.0f;

public:
	void DoAction(class ACharacter* InOwnerCharacter);
	void End_DoAction(class ACharacter* InOwnerCharacter);
};

USTRUCT()
struct FDamagedData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float Power;

	UPROPERTY(EditAnywhere)
	FVector2D LaunchXZ;

	UPROPERTY(EditAnywhere)
	float StopTime;

	UPROPERTY(EditAnywhere)
	class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere)
	bool bKnockdown;

	UPROPERTY(EditAnywhere)
	EDamagedStateType StateType;

public:
	void SendDamage(class ACharacter* InAttacker, class AActor* InAttackCauser, class UShapeComponent* InAttackCollision, class ACharacter* InOther, bool bFirstHit = false, float InDamageMultiplier = 1.0f);

	void PlayHitMotion(class ACharacter* InOwnerCharacter);
	void PlayCameraShake(UWorld* InWorld);
	void PlayHitStop(class ACharacter* InOwnerCharacter, class ACharacter* InAttacker);
	void PlayEffect(class ACharacter* InOwnerCharacter, const FVector& InClosetPoint);
	void PlaySound(class ACharacter* InOwnerCharacter);
};

USTRUCT()
struct FWeaponData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UCEquipment* Equipment = nullptr;
	UPROPERTY()
	class ACAttachment* Attachment = nullptr;
	UPROPERTY()
	class ACAttachment* SubAttachment = nullptr;

	UPROPERTY()
	class UCAction_Action* Action = nullptr;

	UPROPERTY()
	class UCAction_Defend* Defend = nullptr;

	UPROPERTY()
	class UCAction_Air* Action_Air = nullptr;

	UPROPERTY()
	class UCAction_DownStrike* Action_DownStrike = nullptr;

	UPROPERTY()
	class UCAction_Airborne* Action_Airborne = nullptr;

	UPROPERTY()
	class UCAction_Finish* Action_Finish = nullptr;

	UPROPERTY()
	class UCAction_Skill* Action_Skill = nullptr;
};

USTRUCT()
struct FDamagedDataEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	bool				   bFirstHit;
	class UShapeComponent* Collision;
	FDamagedData*		   DamagedData;
};

UCLASS()
class U2505_P2_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
};
