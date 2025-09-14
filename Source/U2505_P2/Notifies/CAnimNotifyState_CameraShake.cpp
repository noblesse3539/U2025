#include "Notifies/CAnimNotifyState_CameraShake.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCAnimNotifyState_CameraShake::UCAnimNotifyState_CameraShake()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Blue;
#endif
}

FString UCAnimNotifyState_CameraShake::GetNotifyName_Implementation() const
{
	return "CameraShake";
}

void UCAnimNotifyState_CameraShake::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	InterpAlpha = 0.0f;
}

void UCAnimNotifyState_CameraShake::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	InterpAlpha += FrameDeltaTime;
	float scale = UKismetMathLibrary::FInterpEaseInOut(0, 1, InterpAlpha / EventReference.GetNotify()->GetDuration(), InterpExponent);

	StartCameraShakeWithScale(scale);
}

void UCAnimNotifyState_CameraShake::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(CameraShake);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(MeshComp->GetOwner()->GetWorld(), 0);
	CheckNull(cameraManager);

	cameraManager->StopCameraShake(CameraShakeInst);
}

void UCAnimNotifyState_CameraShake::StartCameraShakeWithScale(float InScale)
{
	CheckNull(CameraShake);
	CheckNull(OwnerCharacter);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(OwnerCharacter->GetWorld(), 0);
	CheckNull(cameraManager);

	CameraShakeInst = cameraManager->StartCameraShake(CameraShake, InScale);
}
