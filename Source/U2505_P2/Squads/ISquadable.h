#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISquadable.generated.h"

UINTERFACE(MinimalAPI)
class UISquadable : public UInterface
{
	GENERATED_BODY()
};

class U2505_P2_API IISquadable
{
	GENERATED_BODY()

public:
	virtual class ACSquad* GetOwnerSquad() = 0;
	virtual void		   SetOwnerSquad(class ACSquad* InOwnerSquad, uint8 InTeamId) = 0;
	virtual uint8		   GetSquadTeamId() = 0;
	virtual void		   SetOrderTarget(class ACharacter* InTarget) = 0;
};
