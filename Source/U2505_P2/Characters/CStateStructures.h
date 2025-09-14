#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CStateStructures.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle = 0,
	Evade,
	Equip,
	Damaged,
	Action,
	Dead,
	Knockdown,
	Getup,
	Defend,
	Max,
};

UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Patrol = 0,
	Equip,
	Approach,
	Damaged,
	Action,
	Knockdown,
	CircleMove,
	Defend,
	Combat,
	Dead,
	None, // 사용하지 않는 값
	Max,  // WAIT
};

UENUM(BlueprintType)
enum class ESubStateType : uint8
{
	DownAttack = 0,
	Max,
};

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	Walk = 0,
	Run,
	Sprint,
	Max,
};

UENUM(BlueprintType)
enum class EBoundaryType : uint8
{
	Begin = 0,
	End,
	Max,
};

UCLASS()
class U2505_P2_API UCStateStructures : public UObject
{
	GENERATED_BODY()
};
