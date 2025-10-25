#pragma once

#include "CoreMinimal.h"
#include "Notifies/CAnimNotify.h"
#include "CAnimNotify_DebugCamera.generated.h"

UCLASS()
class U2505_P2_API UCAnimNotify_DebugCamera : public UCAnimNotify
{
	GENERATED_BODY()

public:
	UCAnimNotify_DebugCamera();

public:
	FString GetNotifyName_Implementation() const override;
	void	Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
