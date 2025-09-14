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
 * �ǰݽ� ���� ���¿� ���� �ǰ� ��Ÿ�ָ� �����ϰų� ��Ÿ�� ����ð� �ʱ�ȭ ���� ����(�ٿ��߿� ������ �ð��� �þ�ų� �� �� �ֵ���
 *
 * Todo
 * ������ ���� ���¿� ���� ��Ÿ�ָ� ��Ͻ�Ų��
 * ����÷��̿��� ���·ε�, OwnerCharacter ����ϰ� GetMontage ȣ�� �� ���°��� ���� ��Ÿ�ָ� ����
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
