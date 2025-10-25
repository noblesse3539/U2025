#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHighlightable.generated.h"

UINTERFACE(MinimalAPI)
class UIHighlightable : public UInterface
{
	GENERATED_BODY()
};

class U2505_P2_API IIHighlightable
{
	GENERATED_BODY()

public:
	virtual void OnHighlight() = 0;
	virtual void OffHighlight() = 0;

};
