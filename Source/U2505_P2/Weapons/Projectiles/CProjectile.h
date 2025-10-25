#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CProjectile.generated.h"

DECLARE_DELEGATE_ThreeParams(FProjectileBeginOverlap, class ACharacter* InCharacter, AActor* InCauser, int32 DamagedIdx);

UENUM()
enum class EProjectileType : uint8
{
	Inelastic = 0, // ∫Ò≈∫º∫
	Elastic,	   // ≈∫º∫
	Max,
};

UCLASS()
class U2505_P2_API ACProjectile : public AActor
{
	GENERATED_BODY()

public:
	ACProjectile();

private:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	class UParticleSystem* ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	FTransform Explosion_Transform = FTransform::Identity;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	class UParticleSystem* SurroundParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	FTransform Surround_Transform = FTransform::Identity;

	UPROPERTY(EditAnywhere, Category = "Settings")
	EProjectileType Type = EProjectileType::Inelastic;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void Fire(FVector InForward);

public:
	FORCEINLINE void SetDamagedIdx(int32 InIndex) { DamagedIdx = InIndex; }

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	class UParticleSystemComponent* Particle;

private:
	int32 DamagedIdx = 0;

public:
	FProjectileBeginOverlap OnProjectileBeginOverlap;
};
