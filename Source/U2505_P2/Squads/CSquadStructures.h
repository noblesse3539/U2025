#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSquadStructures.generated.h"

UENUM()
enum class ESquadKey {
	None = 0,
	One,
	Two,
	Three,
	Six,
	Twelve,
};

USTRUCT()
struct FSquadType {

	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACEnemy> EnemyClass;

	UPROPERTY(EditAnywhere)
	int32 SquadMemberCount = 1;
};

UCLASS()
class U2505_P2_API UCSquadStructures : public UObject
{
	GENERATED_BODY()
	
};
