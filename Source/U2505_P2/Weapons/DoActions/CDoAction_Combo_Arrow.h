#pragma once

#include "CoreMinimal.h"
#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Interfaces/IShootable.h"
#include "CDoAction_Combo_Arrow.generated.h"

class ACArrow;

UCLASS()
class U2505_P2_API UCDoAction_Combo_Arrow : public UCDoAction_Combo, public IIShootable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	FName SpawnSocket = "Hand_Projectile";

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ACArrow> ArrowClass;

public:
	virtual void BeginDestroy() override;

public:
	void Tick(float InDeltaTime) override {} // ƽ �� �� �������� ȸ���ϴ� ��� ����

	// void DoAction() override;
	// void Begin_DoAction() override;
	// void End_DoAction() override;

public:
	void Shoot() override;
	void Spawn(class USceneComponent* InMesh, FName InSocket, FAttachmentTransformRules InRule) override;

private:
	UFUNCTION()
	void OnArrowHit(class ACharacter* InOther, class UShapeComponent* InCollision, AActor* InCauser, int32 InDamagedIdx);

private:
	TWeakObjectPtr<ACArrow> SpawnedArrow;
};
