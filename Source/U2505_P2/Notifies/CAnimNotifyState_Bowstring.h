#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_Bowstring.generated.h"

UENUM()
enum class EAnimNotify_Bowstring : uint8
{
	Draw,	 // 활 시위 당기기
	Release, // 활 시위 놓기
	Max,
};

class ACAttachment_Bow;

UCLASS()
class U2505_P2_API UCAnimNotifyState_Bowstring : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Type")
	EAnimNotify_Bowstring Type = EAnimNotify_Bowstring::Max;

public:
	UCAnimNotifyState_Bowstring();

protected:
	FString GetNotifyName(FString InName) const;

private:
	FString GetNotifyName_Implementation() const override;

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	TWeakObjectPtr<ACAttachment_Bow> Bow;

private:
	float InterpAlpha;
	float End;
	float InterpSpeed = 1.0f;
	float TotalTime;

private:
	const float Minvalue = 0.0f;
	const float Maxvalue = 1.0f;
};
