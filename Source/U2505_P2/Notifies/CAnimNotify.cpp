#include "Notifies/CAnimNotify.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/CWeaponComponent.h"
FString UCAnimNotify::GetNotifyName(FString InName) const
{
	if (Type != EAnimNotify_Flow::Max)
		return InName + "_" + StaticEnum<EAnimNotify_Flow>()->GetNameStringByValue((int64)Type);

	return InName;
}

UCWeaponComponent* UCAnimNotify::GetWeaponComponent(USkeletalMeshComponent* MeshComp)
{
	CheckNullResult(MeshComp, nullptr);
	CheckNullResult(MeshComp->GetOwner(), nullptr);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	return weapon;
}
