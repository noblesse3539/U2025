#include "Weapons/Actions/CAction_Skill.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/CDoAction.h"

void UCAction_Skill::BeginPlay(ACharacter* InOwnerCharacter, ACAttachment* InAttachment, UCEquipment* InEquipment, const EActionType InActionType)
{
	Super::BeginPlay(InOwnerCharacter, InAttachment, InEquipment, InActionType);

	Collision = FHelpers::GetComponent<UCapsuleComponent>(InAttachment, "Skill", true);
	ensure(!!Collision);

	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UCAction_Skill::Tick(float InDeltaTime)
{
	CheckNull(DoAction);

	DoAction->Tick(InDeltaTime);

	CheckFalse(bMoving);

	FVector location = OwnerCharacter->GetActorLocation();
	location.Z = End.Z;
	float radius = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();

	if (location.Equals(End, radius))
	{
		bMoving = false;
		Start = End = OwnerCharacter->GetActorLocation();

		return;
	}

	FVector direction = (End - Start).GetSafeNormal2D();
	OwnerCharacter->AddActorWorldOffset(direction * Speed, false);
}

void UCAction_Skill::Pressed()
{
	CheckNull(DoAction);
	Super::Pressed();

	DoAction->DoAction();
}

void UCAction_Skill::Released()
{
	Super::Released();

	CheckNull(DoAction);
	DoAction->End_DoAction();
}

void UCAction_Skill::Begin_Action_Implementation()
{
	Super::Begin_Action_Implementation();

	bMoving = true;

	Start = OwnerCharacter->GetActorLocation();

	FRotator rotator = OwnerCharacter->GetActorRotation();
	End = Start + rotator.Vector() * Distance;
	End.Z = Start.Z;

	float radius = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius();
	float height = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	EDrawDebugTrace::Type type = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	FHitResult lineHitResult;
	UKismetSystemLibrary::LineTraceSingle(OwnerCharacter->GetWorld(), Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ignores, type, lineHitResult, true, FLinearColor::Blue);

	if (lineHitResult.bBlockingHit)
	{
		FVector direction = (End - Start).GetSafeNormal2D();
		End = lineHitResult.Location - (direction * radius * 2.0f);
		if (bDebug)
			DrawDebugSphere(OwnerCharacter->GetWorld(), End, radius * 2, 20, FColor::Magenta, false, 5);
	}

	if (bDebug)
		DrawDebugDirectionalArrow(OwnerCharacter->GetWorld(), Start, End, 25, FColor::Purple, false, 5, 0, 3);

	TArray<FHitResult> boxHitResults;

	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(EObjectTypeQuery::ObjectTypeQuery3);

	UKismetSystemLibrary::BoxTraceMultiForObjects(OwnerCharacter->GetWorld(), Start, End, FVector(0.0f, radius, height), rotator, objects, false, ignores, type, boxHitResults, true);
	for (const FHitResult& hitResult : boxHitResults)
	{
		ACharacter* character = Cast<ACharacter>(hitResult.GetActor());
		if (character == nullptr)
			continue;

		FPawnCollisionName* find = Overlapped.FindByPredicate([&](const FPawnCollisionName& InItem) {
			return InItem.Character == character;
		});
		if (find != nullptr)
			continue;

		FPawnCollisionName collision = {
			character->GetCapsuleComponent()->GetCollisionProfileName(),
			character,
		};
		Overlapped.Add(collision);

		character->GetCapsuleComponent()->SetCollisionProfileName("Skill");
	}

	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void UCAction_Skill::End_Action_Implementation()
{
	Super::End_Action_Implementation();

	bMoving = false;

	for (const FPawnCollisionName& collision : Overlapped)
		collision.Character->GetCapsuleComponent()->SetCollisionProfileName(collision.Name);

	Overlapped.Empty();

	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Released();
}