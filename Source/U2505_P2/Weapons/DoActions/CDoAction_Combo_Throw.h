#pragma once

#include "CoreMinimal.h"
#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Interfaces/IShootable.h"
#include "CDoAction_Combo_Throw.generated.h"

class ACProjectile;

UCLASS()
class U2505_P2_API UCDoAction_Combo_Throw : public UCDoAction_Combo, public IIShootable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	FName SpawnSocket = "Hand_Projectile";

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ACProjectile> ProjectileClass;

public:
	void Tick(float InDeltaTime) override {} // 틱 때 적 방향으로 회전하는 모션 방지

	// void DoAction() override;
	// void Begin_DoAction() override;
	// void End_DoAction() override;

public:
	void Shoot() override;
	void Spawn(class USceneComponent* InMesh, FName InSocket, FAttachmentTransformRules InRule) override;

private:
	UFUNCTION()
	void OnProjectileBeginOverlap(class ACharacter* InCharacter, class AActor* InCauser, int32 DamagedIdx);
};
