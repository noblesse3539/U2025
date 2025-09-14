#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponTraceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FWeaponTraceBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCauser, UShapeComponent*, InAttackCollision, class ACharacter*, InOther);

UCLASS()
class U2505_P2_API UCWeaponTraceComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDebug;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	FLinearColor TraceColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	float DrawTime = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	bool bEnable = true;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	FName StartSocket = "TraceStart";
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	FName EndSocket = "TraceEnd";

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	int32 TraceCnt = 5;

public:
	UCWeaponTraceComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
	void OnTraceEnable();

	UFUNCTION()
	void OffTraceEnable();

private:
	void ResetStarts();

private:
	bool bTraceEnable;
	bool bIsValid;

private:
	UPROPERTY()
	class ACAttachment* OwnerWeapon;

	UPROPERTY()
	class USkeletalMeshComponent* OwnerMesh;

private:
	TArray<FVector> Starts;

public:
	FWeaponTraceBeginOverlap OnWeaponTraceBeginOverlap;
};
