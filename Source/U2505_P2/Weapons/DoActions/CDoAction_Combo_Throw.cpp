#include "Weapons/DoActions/CDoAction_Combo_Throw.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/Projectiles/CProjectile.h"
#include "Components/CTargetComponent.h"
#include "Weapons/CAttachment.h"

void UCDoAction_Combo_Throw::OnProjectileBeginOverlap(ACharacter* InCharacter, AActor* InCauser, int32 DamagedIdx)
{
	CheckFalse(DamagedDatas.Num() > 0);
	CheckFalse(DamagedIdx < DamagedDatas.Num());
	CheckNotValid(OwnerCharacter);
	CheckTrue(OwnerCharacter.Get() == InCharacter);

	DamagedDatas[DamagedIdx].SendDamage(OwnerCharacter.Get(), InCauser, nullptr, InCharacter, true);
}

void UCDoAction_Combo_Throw::Shoot()
{
	CheckNotValid(OwnerCharacter);
	CheckNull(ProjectileClass);

	FVector	 location = OwnerCharacter.Get()->GetMesh()->GetSocketLocation(SpawnSocket);
	FRotator rotator = OwnerCharacter.Get()->GetControlRotation();

	FTransform transform;

	UCTargetComponent* targetComp = FHelpers::GetComponent<UCTargetComponent>(OwnerCharacter.Get());

	if (!!targetComp && !!targetComp->GetTarget())
	{
		ACharacter* target = targetComp->GetTarget();

		rotator = UKismetMathLibrary::FindLookAtRotation(location, target->GetActorLocation());
	}

	transform.SetLocation(location);
	transform.SetRotation(FQuat(rotator));

	ACProjectile* projectile = OwnerCharacter.Get()->GetWorld()->SpawnActorDeferred<ACProjectile>(ProjectileClass, transform, OwnerCharacter.Get(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	projectile->OnProjectileBeginOverlap.BindUObject(this, &UCDoAction_Combo_Throw::OnProjectileBeginOverlap);
	projectile->SetDamagedIdx(Index);
	projectile->FinishSpawning(transform);

	projectile->Fire(rotator.Vector());
}

void UCDoAction_Combo_Throw::Spawn(USceneComponent* InMesh, FName InSocket, FAttachmentTransformRules InRule)
{
}
