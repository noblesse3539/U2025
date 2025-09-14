#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDamagedMontageComponent.generated.h"

struct FMontageSet
{
	class UAnimMontage* Montage = nullptr;
	float				PlayRate = 0.0f;
	bool				bNockdown;
};
/***
 * 피격시 현재 상태에 따라 피격 몽타주를 선택하거나 몽타주 실행시간 초기화 등을 관리(다운중에 맞으면 시간이 늘어나거나 할 수 있도록
 *
 * Todo
 * 에셋을 통해 상태에 따른 몽타주를 등록시킨다
 * 비긴플레이에서 에셋로드, OwnerCharacter 등록하고 GetMontage 호출 시 상태값에 따라 몽타주를 실행
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class U2505_P2_API UCDamagedMontageComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* IdleMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	float IdleMontage_PlayRate = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* KnockdownMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	float KnockdownMontage_PlayRate = 1.0f;

public:
	UCDamagedMontageComponent();

protected:
	virtual void BeginPlay() override;

public:
	FMontageSet GetMontageSet();

private:
	ACharacter* OwnerCharacter;
};
