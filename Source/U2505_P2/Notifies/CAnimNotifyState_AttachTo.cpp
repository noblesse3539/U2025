#include "Notifies/CAnimNotifyState_AttachTo.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"

UCAnimNotifyState_AttachTo::UCAnimNotifyState_AttachTo()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Magenta;
#endif
}

FString UCAnimNotifyState_AttachTo::GetNotifyName_Implementation() const
{
	return "AttachTo";
}

void UCAnimNotifyState_AttachTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	UCWeaponComponent* weapon = GetWeaponComponent(MeshComp);
	CheckNull(weapon);

	weapon->AttachTo(BeginSocket);
}

void UCAnimNotifyState_AttachTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	UCWeaponComponent* weapon = GetWeaponComponent(MeshComp);
	CheckNull(weapon);

	weapon->AttachTo(EndSocket);
}

UCWeaponComponent* UCAnimNotifyState_AttachTo::GetWeaponComponent(USkeletalMeshComponent* MeshComp)
{
	CheckNullResult(MeshComp, nullptr);
	CheckNullResult(MeshComp->GetOwner(), nullptr);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	return weapon;
}