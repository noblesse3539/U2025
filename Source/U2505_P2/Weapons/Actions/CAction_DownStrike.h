#pragma once

#include "CoreMinimal.h"
#include "Weapons/CActionBase.h"
#include "CAction_DownStrike.generated.h"

UCLASS(Blueprintable)
class U2505_P2_API UCAction_DownStrike : public UCActionBase
{
	GENERATED_BODY()

public:
	UCAction_DownStrike();

public:
	void Pressed() override;
};
