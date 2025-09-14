#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "ICharacterState.h"

#include "CPlayer.generated.h"

UCLASS()
class U2505_P2_API ACPlayer : public ACharacter, public IGenericTeamAgentInterface, public IICharacterState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Team")
	FGenericTeamId TeamID = 1;

private:
	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* DeadMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	float DeadMontage_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* GetupMontage; // 몽타주 끝에 노티파이로 Getup호출 필수

	UPROPERTY(EditAnywhere, Category = "Montages")
	float GetupMontage_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float AirHitZVelocity = 280.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float DefenceDamageReduceRate = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	int32 PossessCnt = 2;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector2D PitchAngle = FVector2D(-40.0f, +40.0f);

private:
	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* Movement;

private:
	UPROPERTY(VisibleAnywhere)
	class UCZoomComponent* Zoom;

private:
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;

private:
	float KnockdownTime = 4.0f;

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	struct FTakeDamagedEvent
	{
		float Power;

		class ACharacter* Attacker;
		class AActor*	  AttackCauser;

		const struct FDamagedDataEvent* Event;

	} TakeDamagedData;

private:
	void Damage();
	void Dead();
	void Knockdown();

private:
	void Damaged_PlayMontage();
	void Damaged_PlayEffect();
	void Damaged_PlayHisStop();
	void Damaged_PlayCameraShake();
	void Damaged_PlaySound();
	void Damaged_Launch();
	void Damaged_LookAt();


public:
	UFUNCTION()
	void End_Dead() override;

	UFUNCTION()
	void End_Damaged() override;

	UFUNCTION()
	void End_Knockdown() override;

	UFUNCTION()
	void End_Getup() override;

public:
	void		   SetGenericTeamId(const FGenericTeamId& InTeamID) override;
	FGenericTeamId GetGenericTeamId() const override;

public:
	virtual void Landed(const FHitResult& Hit) override;

public:
	bool IsPosessable();
	void Possess();
	void DisPossess();

public:
	void DecreaseStamina(float InAmount);

private:
	class UWidgetComponent* HP_Widget;
	class UCUserWidget_HP*	HealthPoint;
};
