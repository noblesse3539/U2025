#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"

UCLASS()
class U2505_P2_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent* SkeletalMesh;

public:
	ACAttachment_Bow();

public:
	FORCEINLINE float GetDrawBowstring() { return DrawBowstring; }
	FORCEINLINE void  SetDrawBowstring(float InDrawBowstring) { DrawBowstring = InDrawBowstring; }

private:
	float DrawBowstring; // 0 ~ 1
};
