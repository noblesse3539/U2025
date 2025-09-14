#include "Notifies/CAnimNotify_Combo.h"
#include "Notifies/CAnimNotify_Equip.h"
#include "Global.h"

#include "Components/CWeaponComponent.h"

#include "Weapons/CActionBase.h"
#include "Weapons/DoActions/CDoAction_Combo.h"

UCAnimNotify_Combo::UCAnimNotify_Combo()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Emerald;
#endif
}

FString UCAnimNotify_Combo::GetNotifyName_Implementation() const
{
	return GetNotifyName("Combo");
}

void UCAnimNotify_Combo::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UCWeaponComponent* weapon = GetWeaponComponent(MeshComp);
	CheckNull(weapon);

	UCActionBase* action = weapon->GetAction();
	CheckNull(action);

	UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(action->GetDoAction());

	CheckNull(combo);

	switch (Type)
	{
		case EAnimNotify_Flow::Begin:
			combo->OnEnableCombo();
			return;
		case EAnimNotify_Flow::End:
			combo->OffEnableCombo();
			return;
	}
}
