#pragma once

#include "CoreMinimal.h"
#include "Weapons/CActionBase.h"
#include "CAction_Airborne.generated.h"

UCLASS(Blueprintable)
class U2505_P2_API UCAction_Airborne : public UCActionBase
{
	GENERATED_BODY()
	
public:
	UCAction_Airborne();

public:
	void Pressed() override;
};
