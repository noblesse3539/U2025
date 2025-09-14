// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CSkillComponent.generated.h"

UENUM()
enum class ESkillEffectType : uint8
{
	Stun = 0,
	Knockup,
	Knockdown,
};

USTRUCT()
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float Power;

	UPROPERTY(EditAnywhere)
	float Launch;

	UPROPERTY(EditAnywhere)
	float StopTime;

	UPROPERTY(EditAnywhere)
	class USoundWave* Sound = nullptr;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Effect = nullptr;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShake = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<ESkillEffectType> SkillEffects;

public:
	void SendDamage(class ACharacter* InAttacker, class AActor* InAttackCauser, class UShapeComponent* InAttackCollision, class ACharacter* InOther, bool bFirstHit);

	void PlayHitMotion(class ACharacter* InOwnerCharacter);
	void PlayCameraShake(UWorld* InWorld);
	void PlayHitStop(class ACharacter* InOwnerCharacter, class ACharacter* InAttacker);
	void PlayEffect(class ACharacter* InOwnerCharacter, const FVector& InClosetPoint);
};

/*
		USTRUCT()
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	FVector HalfSize = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	float Distance = 0.0f;

	UPROPERTY(EditAnywhere)
	float OffsetZ = 0.0f;

	UPROPERTY(EditAnywhere)
	TArray<ESkillEffectType> SkillEffects;

};

*/

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class U2505_P2_API UCSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	TArray<UDataTable*> Skills;

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebug;

public:
	UCSkillComponent();

protected:
	virtual void BeginPlay() override;

public:
	void DoAction(int32 InSkillIdx = 0);

private:
	ACharacter* OwnerCharacter;

private:
	uint8 ComboIdx = 0;
};
