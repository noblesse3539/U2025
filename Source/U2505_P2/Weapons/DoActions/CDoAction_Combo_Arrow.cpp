#include "Weapons/DoActions/CDoAction_Combo_Arrow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/Projectiles/CArrow.h"
#include "Components/CTargetComponent.h"
#include "Weapons/CAttachment.h"

void UCDoAction_Combo_Arrow::BeginDestroy()
{
	Super::BeginDestroy();
}

void UCDoAction_Combo_Arrow::Shoot()
{
	CheckNotValid(OwnerCharacter);
	CheckNull(ArrowClass);
	CheckNotValid(SpawnedArrow);

	FVector	 location = OwnerCharacter.Get()->GetMesh()->GetSocketLocation(SpawnSocket);
	FRotator rotator = OwnerCharacter.Get()->GetControlRotation();

	SpawnedArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	UCTargetComponent* targetComp = FHelpers::GetComponent<UCTargetComponent>(OwnerCharacter.Get());

	if (!!targetComp && !!targetComp->GetTarget())
	{
		ACharacter* target = targetComp->GetTarget();

		rotator = UKismetMathLibrary::FindLookAtRotation(location, target->GetActorLocation());
	}

	SpawnedArrow->Fire(rotator.Vector());
	SpawnedArrow = nullptr;
}

void UCDoAction_Combo_Arrow::Spawn(USceneComponent* InMesh, FName InSocket, FAttachmentTransformRules InRule)
{
	CheckNotValid(OwnerCharacter);
	CheckNull(InMesh);
	CheckNull(ArrowClass);

	if (!SpawnedArrow.IsValid())
	{
		FTransform transform;
		SpawnedArrow = OwnerCharacter.Get()->GetWorld()->SpawnActorDeferred<ACArrow>(ArrowClass, transform, OwnerCharacter.Get(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		SpawnedArrow->OnArrowHit.BindUObject(this, &UCDoAction_Combo_Arrow::OnArrowHit);
		SpawnedArrow->SetDamagedIdx(Index);
		SpawnedArrow->FinishSpawning(transform);
	}

	SpawnedArrow->AttachToComponent(InMesh, InRule, InSocket);
}

void UCDoAction_Combo_Arrow::OnArrowHit(ACharacter* InOther, UShapeComponent* InCollision, AActor* InCauser, int32 InDamagedIdx)
{
	CheckFalse(DamagedDatas.Num() > 0);
	CheckFalse(InDamagedIdx < DamagedDatas.Num());
	CheckNotValid(OwnerCharacter);
	CheckTrue(OwnerCharacter.Get() == InOther);

	DamagedDatas[InDamagedIdx].SendDamage(OwnerCharacter.Get(), InCauser, nullptr, InOther, true);
}
