#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify.generated.h"

UENUM()
enum class EAnimNotify_Flow : uint8
{
	Begin,
	End,
	Max,
};

UCLASS(Abstract, NotBlueprintable)
class U2505_P2_API UCAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Type")
	EAnimNotify_Flow Type = EAnimNotify_Flow::Max;

protected:
	FString GetNotifyName(FString InName) const;

protected:
	class UCWeaponComponent* GetWeaponComponent(class USkeletalMeshComponent* MeshComp);
};
