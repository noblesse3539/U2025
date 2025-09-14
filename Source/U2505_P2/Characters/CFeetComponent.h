#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData {
	
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Feet")
	FVector LeftDistance; // ���� ���� X ���� (���� ��� ��ǥ�� ���� X�� ������ Z������)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FVector RightDistance; // ���� ���� X

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FVector PelvisDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FRotator LeftRotation; // �޹� ȸ����

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FRotator RightRotation; // ������ ȸ����
};

UCLASS( ClassGroup=(FeetIK), meta=(BlueprintSpawnableComponent) )
class U2505_P2_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category="Debug")
	bool bDebug;

	UPROPERTY(EditAnywhere, Category = "Bone")
	FName LeftBoneName = "FOOT_L";

	UPROPERTY(EditAnywhere, Category = "Bone")
	FName RightBoneName = "FOOT_R";

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float OffsetDistance = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float InterpSpeed = 50.0f;

public:
	const FFeetData& GetData() { return Data; }

public:	
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void Trace(FName InName, float& OutDistance, FRotator& OutRotation);

private:
	class ACharacter* OwnerCharacter;
	FFeetData Data;

};
