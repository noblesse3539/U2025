#pragma once

#include "CoreMinimal.h"
#include "Notifies/CAnimNotify.h"
#include "CAnimNotify_Collision.generated.h"

UCLASS()
class U2505_P2_API UCAnimNotify_Collision : public UCAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Collision")
	FName CollisionName = "Capsule";

public:
	UCAnimNotify_Collision();

private:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
