#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_CameraShake.generated.h"

UCLASS()
class U2505_P2_API UCAnimNotifyState_CameraShake : public UAnimNotifyState
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<class UCameraShakeBase> CameraShake;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float InterpExponent = 2.0f;

public:
	UCAnimNotifyState_CameraShake();

private:
	FString GetNotifyName_Implementation() const override;

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	void StartCameraShakeWithScale(float InScale);

private:
	class UCameraShakeBase* CameraShakeInst;

	UPROPERTY()
	ACharacter* OwnerCharacter;

	float InterpAlpha;
};
