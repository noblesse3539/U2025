#include "Components/CWeaponTraceComponent.h"
#include "Global.h"
#include "Weapons/CAttachment.h"
#include "Components/SkeletalMeshComponent.h"

UCWeaponTraceComponent::UCWeaponTraceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.05f;
}

void UCWeaponTraceComponent::BeginPlay()
{
	Super::BeginPlay();
	CheckFalse(bEnable);

	OwnerWeapon = Cast<ACAttachment>(GetOwner());
	CheckNull(OwnerWeapon);

	OwnerMesh = FHelpers::GetComponent<USkeletalMeshComponent>(OwnerWeapon);
	CheckNull(OwnerMesh);

	CheckFalse(OwnerMesh->DoesSocketExist(StartSocket));
	CheckFalse(OwnerMesh->DoesSocketExist(EndSocket));

	bIsValid = true;

	OwnerWeapon->OnAttachmentBeginCollision.AddDynamic(this, &UCWeaponTraceComponent::OnTraceEnable);
	OwnerWeapon->OnAttachmentEndCollision.AddDynamic(this, &UCWeaponTraceComponent::OffTraceEnable);
}

void UCWeaponTraceComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnWeaponTraceBeginOverlap.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
}

void UCWeaponTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckFalse(bTraceEnable);
	CheckFalse(bIsValid);

	CheckNull(OwnerMesh);
	CheckNull(OwnerWeapon);

	FVector start = OwnerMesh->GetSocketLocation(StartSocket);
	FVector end = OwnerMesh->GetSocketLocation(EndSocket);

	TArray<FVector> traceEnds;
	for (int32 i = 0; i < TraceCnt; i++)
	{
		float alpha = static_cast<float>(i) / static_cast<float>(TraceCnt);

		FVector traceEnd = FMath::Lerp(start, end, alpha);
		traceEnds.Add(traceEnd);
	}

	if (Starts.Num() == 0)
	{
		Starts = traceEnds;
		return;
	}

	TArray<AActor*> ignores;
	ignores.Add(OwnerWeapon);

	EDrawDebugTrace::Type type = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	for (int32 i = 0; i < TraceCnt; i++)
	{
		TArray<FHitResult> hits;
		UKismetSystemLibrary::LineTraceMultiByProfile(OwnerWeapon->GetWorld(), Starts[i], traceEnds[i], "Pawn", false, ignores, type, hits, true, TraceColor, FLinearColor::Blue, DrawTime);

		if (hits.Num() == 0)
			continue;

		if (OnWeaponTraceBeginOverlap.IsBound())
		{
			for (FHitResult hit : hits)
			{
				ACharacter* other = Cast<ACharacter>(hit.GetComponent()->GetOwner());
				if (other == nullptr)
					continue;

				OnWeaponTraceBeginOverlap.Broadcast(nullptr, OwnerWeapon, nullptr, other);
			}
		}
	}

	Starts = traceEnds;
}

void UCWeaponTraceComponent::OnTraceEnable()
{
	ResetStarts();
	bTraceEnable = true;
}

void UCWeaponTraceComponent::OffTraceEnable()
{
	bTraceEnable = false;
	ResetStarts();
}

void UCWeaponTraceComponent::ResetStarts()
{
	Starts.Empty();
}
