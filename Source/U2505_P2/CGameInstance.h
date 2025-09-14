
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"

UCLASS()
class U2505_P2_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	TMap<int32, class ACSquad*> SquadMap;
};
