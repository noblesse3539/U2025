#include "Weapons/Projectiles/CArrow.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACArrow::ACArrow()
{
	FHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	FHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Capsule);
	FHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &ProjectileMovement, "ProjectileMovement");

	Capsule->SetCapsuleRadius(8);
	Capsule->SetCapsuleHalfHeight(8);
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;
	// Capsule->SetCollisionProfileName("BlockAll");

	UStaticMesh* mesh;
	FHelpers::GetAsset<UStaticMesh>(&mesh, "/Script/Engine.StaticMesh'/Game/Meshes/Bow/ArrowMesh.ArrowMesh'");
	Mesh->SetStaticMesh(mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement->InitialSpeed = 5000.0f;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);

	ProjectileMovement->SetActive(false);
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnArrowHit.Unbind();

	if (IsValid(Capsule))
	{
		Capsule->OnComponentHit.RemoveDynamic(this, &ACArrow::OnComponentHit);
	}

	Super::EndPlay(EndPlayReason);
}

void ACArrow::Fire(FVector InForward)
{
	ProjectileMovement->Velocity = InForward * ProjectileMovement->InitialSpeed;
	ProjectileMovement->SetActive(true);
	Capsule->SetCollisionProfileName("BlockAll");

	CheckNull(SurroundParticle);

	UParticleSystemComponent* particleSystemComp = UGameplayStatics::SpawnEmitterAttached(SurroundParticle, RootComponent, NAME_None, Surround_Transform.GetLocation(), Surround_Transform.Rotator(), EAttachLocation::KeepRelativeOffset);
	if (!!particleSystemComp)
	{
		particleSystemComp->SetRelativeScale3D(Surround_Transform.GetScale3D());
	}
}

void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(LifeSpanAfterHit);

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (!!character)
	{
		OnArrowHit.ExecuteIfBound(character, Capsule, this, DamagedIdx);
	}

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
