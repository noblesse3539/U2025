#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SceneComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CWeaponTraceComponent.h"
#include "Weapons/CEquipment.h"

ACAttachment::ACAttachment()
{
	FHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	FHelpers::CreateActorComponent<UCWeaponTraceComponent>(this, &WeaponTrace, "WeaponTrace");
}

void ACAttachment::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);

	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);
		if (shape == nullptr)
			continue;

		shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);

		Collisions.Add(shape);
	}

	OffCollision();

	if (Equipment.IsValid())
	{
		Equipment->OnEquipmentBeginEquip.AddDynamic(this, &ACAttachment::OnEquipmentBeginEquip);
		Equipment->OnEquipmentUnequip.AddDynamic(this, &ACAttachment::OnEquipmentUnequip);
	}
}

void ACAttachment::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	OnAttachmentBeginCollision.RemoveAll(this);
	OnAttachmentEndCollision.RemoveAll(this);

	OnAttachmentBeginOverlap.RemoveAll(this);
	OnAttachmentEndOverlap.RemoveAll(this);

	for (UShapeComponent* shape : Collisions)
	{
		if (!IsValid(shape))
			continue;

		shape->OnComponentBeginOverlap.RemoveDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		shape->OnComponentEndOverlap.RemoveDynamic(this, &ACAttachment::OnComponentEndOverlap);
	}

	if (Equipment.IsValid())
	{
		Equipment->OnEquipmentBeginEquip.RemoveDynamic(this, &ACAttachment::OnEquipmentBeginEquip);
		Equipment->OnEquipmentUnequip.RemoveDynamic(this, &ACAttachment::OnEquipmentUnequip);
	}

	Super::EndPlay(EndPlayReason);
}

void ACAttachment::AttachTo(FName InSocketName)
{
	CheckNull(OwnerCharacter);
	FHelpers::AttachTo(this, OwnerCharacter->GetMesh(), InSocketName);
}

void ACAttachment::AttachToCollision(FName InCollisionName)
{
	for (UShapeComponent* collision : Collisions)
	{
		if (collision->GetName() == InCollisionName)
		{
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InCollisionName);

			return;
		}
	}
}

void ACAttachment::SetEquipment(UCEquipment* InEquipment)
{
	Equipment = InEquipment;
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == OwnerCharacter);
	CheckNull(Cast<UShapeComponent>(OverlappedComponent));
	CheckNull(Cast<ACharacter>(OtherActor));

	if (OnAttachmentBeginOverlap.IsBound())
	{
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<UShapeComponent>(OverlappedComponent), Cast<ACharacter>(OtherActor));
	}
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OtherActor == OwnerCharacter);
	CheckNull(Cast<ACharacter>(OtherActor));

	if (OnAttachmentEndOverlap.IsBound())
	{
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor));
	}
}

void ACAttachment::OnCollision(FName InName)
{
	if (OnAttachmentBeginCollision.IsBound())
	{
		OnAttachmentBeginCollision.Broadcast();
	}

	if (InName.IsValid())
	{
		for (UShapeComponent* component : Collisions)
		{
			if (component->GetFName() == InName)
			{
				component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

				return;
			}
		}
	}

	for (UShapeComponent* component : Collisions)
	{
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ACAttachment::OffCollision()
{
	if (OnAttachmentEndCollision.IsBound())
	{
		OnAttachmentEndCollision.Broadcast();
	}

	for (UShapeComponent* collision : Collisions)
	{
		collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
