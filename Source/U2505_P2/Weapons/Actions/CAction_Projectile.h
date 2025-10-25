#pragma once

#include "CoreMinimal.h"
#include "Weapons/Actions/CAction_Action.h"

#include "CAction_Projectile.generated.h"

class ACProjectile;

UCLASS(Blueprintable)
class U2505_P2_API UCAction_Projectile : public UCAction_Action

{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDebug;

public:
	UCAction_Projectile();
	virtual void BeginPlay(class ACharacter* InOwnerCharacter, class ACAttachment* InAttachment, class UCEquipment* InEquipment, const EActionType InActionType);
	void		 Tick(float InDeltaTime) override;
	void		 Pressed() override;
	void		 Released() override;
};
