#include "Notifies/CAnimNotifyState_Action.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CActionBase.h"

UCAnimNotifyState_Action::UCAnimNotifyState_Action()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Silver;
#endif
}

FString UCAnimNotifyState_Action::GetNotifyName_Implementation() const
{
	return "State_Action";
}

void UCAnimNotifyState_Action::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAction());

	weapon->GetAction()->Begin_Action();
}

void UCAnimNotifyState_Action::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAction());

	weapon->GetAction()->End_Action();
}