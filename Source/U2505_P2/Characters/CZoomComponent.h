#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CZoomComponent.generated.h"

UCLASS(ClassGroup = (Zoom), meta = (BlueprintSpawnableComponent))
class U2505_P2_API UCZoomComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float Speed = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FVector2D Range = FVector2D(100, 500);

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float InterpSpeed = 5.0;

public:
	UCZoomComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class ACharacter*		   OwnerCharacter;
	class USpringArmComponent* SpringArm;

public:
	void SetValue(float InZoomValue);

private:
	float TargetArmLength;
};
