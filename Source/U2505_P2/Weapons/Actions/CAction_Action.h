#pragma once

#include "CoreMinimal.h"
#include "Weapons/CActionBase.h"
#include "CAction_Action.generated.h"

UCLASS(Blueprintable)
class U2505_P2_API UCAction_Action : public UCActionBase
{
	GENERATED_BODY()
	
public:
	void Tick(float InDeltaTime) override;
	void Pressed() override;
	void Released() override;
};
