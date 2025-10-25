#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IShootable.generated.h"

UINTERFACE(MinimalAPI)
class UIShootable : public UInterface
{
	GENERATED_BODY()
};

class U2505_P2_API IIShootable
{
	GENERATED_BODY()

public:
	virtual void Shoot() = 0;
	virtual void Spawn(class USceneComponent* InMesh, FName InSocket, FAttachmentTransformRules InRule) = 0;
};
