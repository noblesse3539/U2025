#include "Notifies/CAnimNotify_FireProjectile.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CActionBase.h"
#include "Interfaces/IShootable.h"
#include "Weapons/CDoAction.h"

UCAnimNotify_FireProjectile::UCAnimNotify_FireProjectile()
{
}

FString UCAnimNotify_FireProjectile::GetNotifyName_Implementation() const
{
	return GetNotifyName("FireProjectile");
}

void UCAnimNotify_FireProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	UCActionBase* action = weapon->GetAction();
	CheckNull(action);

	UCDoAction* doAction = action->GetDoAction();
	CheckNull(doAction);

	IIShootable* shootable = Cast<IIShootable>(doAction);
	CheckNull(shootable);

	shootable->Shoot();
}
