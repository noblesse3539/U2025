#include "Notifies/CAnimNotify_DoAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"

#include "Weapons/CDoAction.h"
#include "Weapons/CActionBase.h"

UCAnimNotify_DoAction::UCAnimNotify_DoAction()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Cyan;
#endif
}

FString UCAnimNotify_DoAction::GetNotifyName_Implementation() const
{
	return GetNotifyName("DoAction");
}

void UCAnimNotify_DoAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UCWeaponComponent* weapon = GetWeaponComponent(MeshComp);
	CheckNull(weapon);

	UCActionBase* action = weapon->GetAction();
	CheckNull(action);

	UCDoAction* doAction = action->GetDoAction();
	CheckNull(doAction);

	switch (Type)
	{
		case EAnimNotify_Flow::Begin:
			doAction->Begin_DoAction();
			return;
		case EAnimNotify_Flow::End:
			doAction->End_DoAction();
			return;
	}
}
