#include "Notifies/CAnimNotify_Equip.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"

UCAnimNotify_Equip::UCAnimNotify_Equip()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Magenta;
#endif
}

FString UCAnimNotify_Equip::GetNotifyName_Implementation() const
{
	return GetNotifyName("Equip");
}

void UCAnimNotify_Equip::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UCWeaponComponent* weapon = GetWeaponComponent(MeshComp);
	CheckNull(weapon);

	switch (Type)
	{
		case EAnimNotify_Flow::Begin:
			weapon->Begin_Equip();
			return;
		case EAnimNotify_Flow::End:
			weapon->End_Equip();
			return;
	}
}
