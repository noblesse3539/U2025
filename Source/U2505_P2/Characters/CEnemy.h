#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ICharacterState.h"
#include "Interfaces/IHighlightable.h"
#include "CEnemy.generated.h"

class UWidgetComponent;
class UCUserWidget_HP;
class UCUserWidget_State;

UCLASS()
class U2505_P2_API ACEnemy : public ACharacter, public IICharacterState, public IIHighlightable
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebug;

private:
	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* DeadMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	float DeadMontage_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* GetupMontage; // 몽타주 끝에 노티파이로 Getup호출 필수

	UPROPERTY(EditAnywhere, Category = "Montages")
	float GetupMontage_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* DefendMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	float DefendMontage_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float AirHitZVelocity = 280.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float DefenceDamageReduceRate = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	class UMaterialInterface* HighlightMaterial;

protected:
	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon;

	// UPROPERTY(VisibleAnywhere)
	// class UCDamagedMontageComponent* DamagedMontageCmp;

private:
	float KnockdownTime = 4.0f;

public:
	UPROPERTY(EditAnywhere)
	FVector StartLocation;

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	void SetDeadMontage(class UAnimMontage* InMontage, float InPlayRate = 1.0f);

protected:
	UFUNCTION()
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

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
	virtual void End_Dead() override;

	UFUNCTION()
	virtual void End_Damaged() override;

	UFUNCTION()
	virtual void End_Knockdown() override;

	UFUNCTION()
	virtual void End_Getup() override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual void OnHighlight() override;
	virtual void OffHighlight() override;

private:
	void InitOverlayMaterial();

private:
	TWeakObjectPtr<UWidgetComponent> HP_Widget;
	TWeakObjectPtr<UCUserWidget_HP>	 HealthPoint = nullptr;

private:
	TWeakObjectPtr<UWidgetComponent>   State_Widget;
	TWeakObjectPtr<UCUserWidget_State> StateForDebug = nullptr;

private:
	bool		 bKnockdown;
	FTimerHandle Handle_Knockdown;

private:
	UPROPERTY()
	class UMaterialInstanceDynamic* HighlightMaterialDynamic;

public:
	UFUNCTION()
	virtual void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);
};
