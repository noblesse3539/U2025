#include "Weapons/Actions/CAction_Projectile.h"
#include "Global.h"
#include "Weapons/CDoAction.h"
#include "Weapons/Projectiles/CProjectile.h"
#include "GameFramework/Character.h"

UCAction_Projectile::UCAction_Projectile()
{
}

void UCAction_Projectile::BeginPlay(ACharacter* InOwnerCharacter, ACAttachment* InAttachment, UCEquipment* InEquipment, const EActionType InActionType)
{
	Super::BeginPlay(InOwnerCharacter, InAttachment, InEquipment, InActionType);
}

void UCAction_Projectile::Tick(float InDeltaTime)
{
	CheckNull(DoAction);

	DoAction->Tick(InDeltaTime);
}

void UCAction_Projectile::Pressed()
{
	Super::Pressed();

	CheckNull(DoAction);
	DoAction->DoAction();
}

void UCAction_Projectile::Released()
{
	Super::Released();

	CheckNull(DoAction);
	DoAction->End_DoAction();
}
