#pragma once

#include "CoreMinimal.h"
#include "Notifies/CAnimNotify.h"
#include "CAnimNotify_Equip.generated.h"

UCLASS()
class U2505_P2_API UCAnimNotify_Equip : public UCAnimNotify
{
	GENERATED_BODY()

public:
	UCAnimNotify_Equip();

private:
	FString GetNotifyName_Implementation() const override;
	void	Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
