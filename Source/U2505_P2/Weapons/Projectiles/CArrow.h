#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CArrow.generated.h"

DECLARE_DELEGATE_FourParams(FArrowHit, class ACharacter* InOther, class UShapeComponent* InCollision, AActor* InCauser, int32 InDamagedIdx);

UCLASS()
class U2505_P2_API ACArrow : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
	float LifeSpanAfterSpawn = 3;

	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
	float LifeSpanAfterHit = 1;

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	class UParticleSystem* SurroundParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	FTransform Surround_Transform = FTransform::Identity;

public:
	ACArrow();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void Fire(FVector InForward);

public:
	FORCEINLINE void SetDamagedIdx(int32 InIndex) { DamagedIdx = InIndex; }

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	class UParticleSystemComponent* Particle;

private:
	int32 DamagedIdx = 0;

public:
	FArrowHit OnArrowHit;
};
