#pragma once

#include "CoreMinimal.h"
#include "Weapons/CActionBase.h"
#include "CAction_Air.generated.h"

UCLASS(Blueprintable)
class U2505_P2_API UCAction_Air : public UCActionBase
{
	GENERATED_BODY()

public:
	UCAction_Air();

public:
	void Tick(float InDeltaTime) override;
	void Pressed() override;
};
