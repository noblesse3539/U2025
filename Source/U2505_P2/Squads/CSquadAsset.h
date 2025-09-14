#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "CSquadStructures.h"

#include "CSquadAsset.generated.h"

UCLASS()
class U2505_P2_API UCSquadAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	FSquadType GetSquadTypes(ESquadKey key);

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<ESquadKey,FSquadType> SquadTypes;
};
