#include "Notifies/CAnimNotify_Collision.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CAttachment.h"

UCAnimNotify_Collision::UCAnimNotify_Collision()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Green;
#endif
}

FString UCAnimNotify_Collision::GetNotifyName_Implementation() const
{
	return GetNotifyName("Collision");
}

void UCAnimNotify_Collision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UCWeaponComponent* weapon = GetWeaponComponent(MeshComp);
	CheckNull(weapon);

	ACAttachment* attachment = weapon->GetAttachment();
	CheckNull(attachment);

	switch (Type)
	{
		case EAnimNotify_Flow::Begin:
			attachment->OnCollision(CollisionName);
			return;
		case EAnimNotify_Flow::End:
			attachment->OffCollision();
			return;
	}
}