#include "Notifies/CAnimNotifyState_Bowstring.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CAttachment.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Weapons/CActionBase.h"
#include "Weapons/CDoAction.h"
#include "Interfaces/IShootable.h"
#include "Components/SkeletalMeshComponent.h"

UCAnimNotifyState_Bowstring::UCAnimNotifyState_Bowstring()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Orange;
#endif
}

FString UCAnimNotifyState_Bowstring::GetNotifyName(FString InName) const
{
	if (Type != EAnimNotify_Bowstring::Max)
		return InName + "_" + StaticEnum<EAnimNotify_Bowstring>()->GetNameStringByValue((int64)Type);

	return InName;
}

FString UCAnimNotifyState_Bowstring::GetNotifyName_Implementation() const
{
	return GetNotifyName("Bowstring");
}

void UCAnimNotifyState_Bowstring::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Bow = nullptr;

	AActor* owner = MeshComp->GetOwner();
	CheckNull(owner);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(owner);
	CheckNull(weapon);

	ACAttachment* attachment = weapon->GetAttachment();
	CheckNull(attachment);

	Bow = Cast<ACAttachment_Bow>(attachment);
	CheckNotValid(Bow);

	UCActionBase* action = weapon->GetAction();
	CheckNull(action);

	IIShootable* shootable = Cast<IIShootable>(action->GetDoAction());
	CheckNull(shootable);

	weapon->OnBowstring();

	if (Type == EAnimNotify_Bowstring::Draw)
	{
		InterpAlpha = Minvalue;
		End = Maxvalue;

		weapon->OnPullBowstring();

		// USkeletalMeshComponent* mesh = FHelpers::GetComponent<USkeletalMeshComponent>(owner);
		// CheckNull(mesh);
		// shootable->Spawn(mesh, "Hand_Arrow", FAttachmentTransformRules::KeepRelativeTransform);

		USkeletalMeshComponent* mesh = FHelpers::GetComponent<USkeletalMeshComponent>(Bow.Get());
		CheckNull(mesh);
		shootable->Spawn(mesh, "Arrow", FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else if (Type == EAnimNotify_Bowstring::Release)
	{

		InterpAlpha = Maxvalue;
		End = Minvalue;

		weapon->OffPullBowstring();

		USkeletalMeshComponent* mesh = FHelpers::GetComponent<USkeletalMeshComponent>(Bow.Get());
		CheckNull(mesh);
		shootable->Spawn(mesh, "Arrow", FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	else
	{
		InterpAlpha = Maxvalue;
		End = Minvalue;

		weapon->OffPullBowstring();
	}

	TotalTime = TotalDuration;
	InterpSpeed = Maxvalue / TotalDuration;
}

void UCAnimNotifyState_Bowstring::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	CheckNotValid(Bow);

	InterpAlpha = UKismetMathLibrary::FInterpTo_Constant(InterpAlpha, End, FrameDeltaTime, InterpSpeed);

	Bow->SetDrawBowstring(InterpAlpha);
}

void UCAnimNotifyState_Bowstring::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* owner = MeshComp->GetOwner();
	CheckNull(owner);

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(owner);
	CheckNull(weapon);

	CheckNotValid(Bow);

	weapon->OffBowstring();

	if (Type == EAnimNotify_Bowstring::Draw)
	{
		Bow->SetDrawBowstring(Maxvalue);
	}
	else if (Type == EAnimNotify_Bowstring::Release)
	{
		Bow->SetDrawBowstring(Minvalue);

		UCActionBase* action = weapon->GetAction();
		CheckNull(action);

		IIShootable* shootable = Cast<IIShootable>(action->GetDoAction());
		CheckNull(shootable);
		shootable->Shoot();
	}
}
