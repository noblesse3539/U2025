#include "Weapons/Projectiles/CProjectile.h"
#include "Global.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACProjectile::ACProjectile()
{
	FHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere");

	FHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &ProjectileMovement, "ProjectileMovement");
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void ACProjectile::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(ProjectileMovement);
	ProjectileMovement->SetActive(false);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACProjectile::OnComponentBeginOverlap);
}

void ACProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnProjectileBeginOverlap.Unbind();

	if (IsValid(Sphere))
	{
		Sphere->OnComponentBeginOverlap.RemoveDynamic(this, &ACProjectile::OnComponentBeginOverlap);
	}

	if (IsValid(Particle))
	{
		Particle->DeactivateSystem();
		Particle->DestroyComponent();
		Particle = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

void ACProjectile::Fire(FVector InForward)
{
	SetLifeSpan(3.0f);
	ProjectileMovement->Velocity = InForward * ProjectileMovement->InitialSpeed;
	ProjectileMovement->SetActive(true);

	CheckNull(SurroundParticle);

	Particle = UGameplayStatics::SpawnEmitterAttached(SurroundParticle, RootComponent, NAME_None, Surround_Transform.GetLocation(), Surround_Transform.Rotator(), EAttachLocation::KeepRelativeOffset);
	if (!!Particle)
	{
		Particle->SetRelativeScale3D(Surround_Transform.GetScale3D());
	}
}

void ACProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(GetOwner() == OtherActor);

	if (!!ExplosionParticle)
	{
		FTransform transform = Explosion_Transform;
		transform.AddToTranslation(SweepResult.Location);
		transform.SetRotation(FQuat(SweepResult.ImpactNormal.Rotation()));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, transform);
	}

	OnProjectileBeginOverlap.ExecuteIfBound(Cast<ACharacter>(OtherActor), this, DamagedIdx);

	Destroy();
}
