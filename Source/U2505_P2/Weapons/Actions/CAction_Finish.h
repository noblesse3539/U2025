#pragma once

#include "CoreMinimal.h"
#include "Weapons/CActionBase.h"
#include "CAction_Finish.generated.h"

UCLASS(Blueprintable)
class U2505_P2_API UCAction_Finish : public UCActionBase
{
	GENERATED_BODY()

public:
	UCAction_Finish();

public:
	void Tick(float InDeltaTime) override;
	void Pressed() override;
};
