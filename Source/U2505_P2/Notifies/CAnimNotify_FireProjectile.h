#pragma once

#include "CoreMinimal.h"
#include "Notifies/CAnimNotify.h"
#include "CAnimNotify_FireProjectile.generated.h"

UCLASS()
class U2505_P2_API UCAnimNotify_FireProjectile : public UCAnimNotify
{
	GENERATED_BODY()

public:
	UCAnimNotify_FireProjectile();
	
private:
	FString GetNotifyName_Implementation() const override;
	void	Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
