#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Bow.generated.h"

class ACAttachment_Bow;

UCLASS()
class U2505_P2_API UCAnimInstance_Bow : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	float DrawBowstring;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	TWeakObjectPtr<ACAttachment_Bow> OwnerBow;
};
