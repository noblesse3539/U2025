#include "Notifies/CAnimNotifyState_TimeDilation.h"
#include "Global.h"
#include "Characters/CPlayer.h"

UCAnimNotifyState_TimeDilation::UCAnimNotifyState_TimeDilation()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::White;
#endif
}

FString UCAnimNotifyState_TimeDilation::GetNotifyName_Implementation() const
{
	return "TimeDilation";
}

void UCAnimNotifyState_TimeDilation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OwnerPlayer = Cast<ACPlayer>(MeshComp->GetOwner());
	InterpAlpha = 0.0f;
}

void UCAnimNotifyState_TimeDilation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	CheckNull(OwnerPlayer);

	InterpAlpha += FrameDeltaTime;
	float timeDilation = UKismetMathLibrary::FInterpEaseInOut(1.0f, 0.0005f, InterpAlpha / EventReference.GetNotify()->GetDuration(), InterpExponent);

	// 게임 전체 속도를 느리게 하고 플레이어는 역수를 취해서 정상속도를 유지
	UGameplayStatics::SetGlobalTimeDilation(OwnerPlayer->GetWorld(), timeDilation);
	OwnerPlayer->CustomTimeDilation = 1.0f / UGameplayStatics::GetGlobalTimeDilation(OwnerPlayer->GetWorld());
}

void UCAnimNotifyState_TimeDilation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(OwnerPlayer);

	// 종료 시 딜레이션 초기화
	UGameplayStatics::SetGlobalTimeDilation(OwnerPlayer->GetWorld(), 1.0f);
	OwnerPlayer->CustomTimeDilation = 1.0f;
}
