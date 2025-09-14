#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacterState.generated.h"

UINTERFACE(MinimalAPI)
class UICharacterState : public UInterface
{
	GENERATED_BODY()
};

class U2505_P2_API IICharacterState
{
	GENERATED_BODY()

public:
	virtual void End_Dead() = 0;
	virtual void End_Damaged() = 0;
	virtual void End_Knockdown() = 0;
	virtual void End_Getup() = 0;
};
