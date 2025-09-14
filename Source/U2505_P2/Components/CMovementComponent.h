#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/CStateStructures.h"
#include "CMovementComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class U2505_P2_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed[(int32)ESpeedType::Max] = { 300, 400, 600 };

private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector2D PitchAngle = FVector2D(-40.0f, 40.0f);

public:
	UCMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	void OnMoveForward(float InValue);
	void OnMoveRight(float InValue);
	void OnHorizontalLook(float InValue);
	void OnVerticalLook(float InValue);

public:
	void OnSprint();
	void OnRun();
	void OnWalk();
	void OnJump();

public:
	FORCEINLINE void OnFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void OffFixedCamera() { bFixedCamera = false; }

	bool IsFalling();

private:
	void SetSpeed(ESpeedType InType);

public:
	FORCEINLINE void Stop() { bCanMove = false; }
	FORCEINLINE void Move() { bCanMove = true; }
	FORCEINLINE bool GetCanMove() { return bCanMove; }

public:
	void OnFixForward();
	void OffFixForward();

private:
	void InitMontages();

private:
	class ACharacter* OwnerCharacter;

private:
	bool bCanMove = true;
	bool bFixedCamera;
};
