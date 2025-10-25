#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CTargetComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class U2505_P2_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebug;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float TraceRadius = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float FinishAngle = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float InterpSpeed = 2.5f;

private:
	UPROPERTY(EditAnywhere, Category = "Cursor")
	class UParticleSystem* Particle;

public:
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void EndTargeting();

public:
	void SetTarget(ACharacter* InTarget);

	ACharacter* GetTarget();

private:
	TWeakObjectPtr<ACharacter> Target;

private:
	TWeakObjectPtr<ACharacter> OwnerCharacter;

private:
	class UParticleSystemComponent* TargetParticle;
};
