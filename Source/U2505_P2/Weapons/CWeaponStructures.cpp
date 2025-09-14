#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CMovementComponent.h"
#include "Components/CDamagedMontageComponent.h"

#include "Particles/ParticleSystem.h"
#include "Characters/CPlayer.h"

void FDoActionData::DoAction(ACharacter* InOwnerCharacter)
{
	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(InOwnerCharacter);
	if (!!movement)
	{
		if (bCanMove == false)
		{
			movement->Stop();
		}
		if (bFixedCamera)
		{
			movement->OnFixedCamera();
		}
	}

	if (!!Montage)
	{
		InOwnerCharacter->PlayAnimMontage(Montage, PlayRate);
	}

	
	if (Cost > 0.0f) {
		ACPlayer* player = Cast<ACPlayer>(InOwnerCharacter);
		CheckNull(player);

		player->DecreaseStamina(Cost);
	}
}

void FDoActionData::End_DoAction(ACharacter* InOwnerCharacter)
{
	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(InOwnerCharacter);
	if (!!movement)
	{
		movement->Move();

		movement->OffFixedCamera();
	}
}

void FDamagedData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, UShapeComponent* InAttackCollision, ACharacter* InOther, bool bFirstHit, float InDamageMultiplier)
{
	CheckNull(InOther);
	CheckNull(InAttacker);
	FDamagedDataEvent e;
	e.bFirstHit = bFirstHit;
	e.Collision = InAttackCollision;
	e.DamagedData = this;
	InOther->TakeDamage(Power * InDamageMultiplier, e, InAttacker->GetInstigatorController(), InAttackCauser);
}

void FDamagedData::PlayHitMotion(ACharacter* InOwnerCharacter)
{
	CheckNull(InOwnerCharacter);

	UCDamagedMontageComponent* damagedMontageComponent = FHelpers::GetComponent<UCDamagedMontageComponent>(InOwnerCharacter);
	if (!!damagedMontageComponent)
	{
		FMontageSet montageSet = damagedMontageComponent->GetMontageSet();
		if (!!montageSet.Montage)
		{
			if (montageSet.bNockdown || !Montage)
			{
				InOwnerCharacter->PlayAnimMontage(montageSet.Montage, montageSet.PlayRate);
				return;
			}
		}
	}
	CheckNull(Montage);
	InOwnerCharacter->PlayAnimMontage(Montage, PlayRate);
}

void FDamagedData::PlayCameraShake(UWorld* InWorld)
{
	CheckNull(InWorld);
	CheckNull(CameraShake);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(InWorld, 0);

	cameraManager->StartCameraShake(CameraShake);
}

void FDamagedData::PlayHitStop(ACharacter* InOwnerCharacter, ACharacter* InAttacker)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));
	CheckNull(InOwnerCharacter);
	CheckNull(InAttacker);

	InOwnerCharacter->CustomTimeDilation = 1e-3f;
	InAttacker->CustomTimeDilation = 1e-3f;

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]() {
		InOwnerCharacter->CustomTimeDilation = 1.0f;
		InAttacker->CustomTimeDilation = 1.0f;
	});

	FTimerHandle handle;
	InOwnerCharacter->GetWorld()->GetTimerManager().SetTimer(handle, timerDelegate, StopTime, false);
}

void FDamagedData::PlayEffect(ACharacter* InOwnerCharacter, const FVector& InClosetPoint)
{
	CheckNull(InOwnerCharacter);
	CheckNull(Effect);

	FVector	 location = InClosetPoint;
	FRotator rotator = InOwnerCharacter->GetActorRotation();

	location += rotator.RotateVector(EffectLocation);

	FTransform transform;
	transform.SetLocation(location);
	transform.SetScale3D(EffectScale);

	UParticleSystem* particle = Cast<UParticleSystem>(Effect);
	UGameplayStatics::SpawnEmitterAtLocation(InOwnerCharacter->GetWorld(), particle, transform);
}

void FDamagedData::PlaySound(ACharacter* InOwnerCharacter)
{
	CheckNull(InOwnerCharacter);
	CheckNull(Sound);

	UGameplayStatics::SpawnSoundAtLocation(InOwnerCharacter->GetWorld(), Sound, InOwnerCharacter->GetActorLocation());
}
