#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_GhostTrail.generated.h"

UCLASS()
class U2505_P2_API UCAnimNotifyState_GhostTrail : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Class")
	TSubclassOf<class ACGhostTrail> GhostTrailClass;

private:
	UPROPERTY(EditAnywhere, Category = "Capture")
	bool bHasChildren = false;

	UPROPERTY(EditAnywhere, Category = "Capture")
	int32 CaptureCount = 0;

	UPROPERTY(EditAnywhere, Category = "Capture")
	float Interval = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Capture")
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category = "Capture")
	float ColorExponent = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Capture")
	FVector Scale = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Interp")
	bool bUseInterp = false;

	UPROPERTY(EditAnywhere, Category = "Interp")
	float InterpStartInterval = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Interp")
	float InterpExponent = 2.0f;

public:
	UCAnimNotifyState_GhostTrail();

private:
	FString GetNotifyName_Implementation() const override;

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	class ACGhostTrail* GhostTrail;

	float InterpAlpha = 0.3f;
};
