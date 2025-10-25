#include "Components/CTargetComponent.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Particles/ParticleSystemComponent.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	FHelpers::GetAsset<UParticleSystem>(&Particle, "/Script/Engine.ParticleSystem'/Game/AdvancedMagicFX09/Particles/P_ky_circle1_loop.P_ky_circle1_loop'");
}

void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNotValid(OwnerCharacter);
	CheckNotValid(Target);

	if (Target->GetDistanceTo(OwnerCharacter.Get()) >= TraceRadius)
	{
		EndTargeting();

		return;
	}

	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target->GetActorLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator current = OwnerCharacter->GetControlRotation();

	rotator.Pitch = current.Pitch;

	if (current.Equals(rotator, FinishAngle))
	{
		OwnerCharacter->GetController()->SetControlRotation(rotator);

		return;
	}

	rotator.Roll = current.Roll;
	rotator = UKismetMathLibrary::RInterpTo(current, rotator, DeltaTime, InterpSpeed);
	OwnerCharacter->GetController()->SetControlRotation(rotator);
}

void UCTargetComponent::EndTargeting()
{
	SetTarget(nullptr);
}

void UCTargetComponent::SetTarget(ACharacter* InTarget)
{
	if (!!TargetParticle)
	{
		TargetParticle->DestroyComponent();
	}

	if (Target == InTarget && Target.IsValid())
	{
		Target = nullptr;

		return;
	}
	Target = InTarget;

	if (Target.IsValid())
	{
		TargetParticle = UGameplayStatics::SpawnEmitterAttached(Particle, InTarget->GetMesh(), "Target_Particle");
	}
}

ACharacter* UCTargetComponent::GetTarget()
{
	CheckNotValidResult(Target, nullptr);

	return Target.Get();
}
