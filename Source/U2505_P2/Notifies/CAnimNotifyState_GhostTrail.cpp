#include "Notifies/CAnimNotifyState_GhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/Etcs/CGhostTrail.h"

UCAnimNotifyState_GhostTrail::UCAnimNotifyState_GhostTrail()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Purple;
#endif

	FHelpers::GetClass<ACGhostTrail>(&GhostTrailClass, "/Script/Engine.Blueprint'/Game/Weapons/Etcs/BP_CGhostTrail.BP_CGhostTrail_C'");
}

FString UCAnimNotifyState_GhostTrail::GetNotifyName_Implementation() const
{
	return "GhostTrail";
}

void UCAnimNotifyState_GhostTrail::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	FTransform transform;
	// transform.SetLocation(character->GetMesh()->GetComponentLocation());
	// transform.SetRotation(FQuat(character->GetMesh()->GetComponentRotation()));

	GhostTrail = MeshComp->GetWorld()->SpawnActorDeferred<ACGhostTrail>(GhostTrailClass, transform, MeshComp->GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	GhostTrail->SetMaterial(Color, ColorExponent);
	GhostTrail->SetMeshScale(Scale);
	GhostTrail->SetInterval(Interval);
	GhostTrail->SetCaptureCount(CaptureCount);
	GhostTrail->SetHasChildrenMeshes(bHasChildren);
	GhostTrail->SetUseInterp(bUseInterp);
	GhostTrail->SetInterpStartInterval(InterpStartInterval);
	GhostTrail->SetDuration(TotalDuration);
	GhostTrail->SetInterpExponent(InterpExponent);

	GhostTrail->FinishSpawning(transform);
}

void UCAnimNotifyState_GhostTrail::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	CheckFalse(MeshComp->GetOwner()->IsA<ACharacter>());

	CheckNull(GhostTrail);

	GhostTrail->MarkedDestroy();
}