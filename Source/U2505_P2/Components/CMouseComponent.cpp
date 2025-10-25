#include "Components/CMouseComponent.h"
#include "Global.h"
#include "Characters/CPlayer.h"

#include "Components/CMovementComponent.h"
#include "Components/CTargetComponent.h"

#include "Interfaces/IHighlightable.h"
#include "Characters/CEnemy.h"

UCMouseComponent::UCMouseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f;
}

void UCMouseComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACPlayer>(GetOwner());
	CheckNotValid(Owner);

	Movement = FHelpers::GetComponent<UCMovementComponent>(Owner.Get());
	TargetComponent = FHelpers::GetComponent<UCTargetComponent>(Owner.Get());
}

void UCMouseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckFalse(bShowCursor);

	CheckNotValid(Owner);

	APlayerController* controller = Owner->GetController<APlayerController>();
	CheckNull(controller);

	ECollisionChannel traceChannel = ECollisionChannel::ECC_GameTraceChannel1;

	FHitResult hitted;
	controller->GetHitResultUnderCursor(traceChannel, false, hitted);

	if (hitted.bBlockingHit)
	{
		SetMouseOver(hitted.GetActor());
	}
	else
	{
		SetMouseOver(nullptr);
	}
}

void UCMouseComponent::ShowCursor()
{
	CheckNotValid(Owner);

	bShowCursor = true;

	APlayerController* controller = Owner->GetController<APlayerController>();
	CheckNull(controller);

	controller->bShowMouseCursor = bShowCursor;
	controller->SetInputMode(FInputModeGameAndUI());

	controller->bEnableMouseOverEvents = true;

	CheckNotValid(Movement);
	Movement->OnFixedCamera();
}

void UCMouseComponent::HideCursor()
{
	CheckNotValid(Owner);

	bShowCursor = false;

	APlayerController* controller = Owner->GetController<APlayerController>();
	CheckNull(controller);

	controller->bShowMouseCursor = bShowCursor;
	controller->SetInputMode(FInputModeGameOnly());

	controller->bEnableMouseOverEvents = false;

	CheckNotValid(Movement);
	Movement->OffFixedCamera();

	SetMouseOver(nullptr);
}

void UCMouseComponent::OnLeftClick()
{
	CheckFalse(bShowCursor);

	CheckNotValid(Owner);
	CheckNotValid(TargetComponent);

	APlayerController* controller = Owner->GetController<APlayerController>();
	CheckNull(controller);

	ECollisionChannel traceChannel = ECollisionChannel::ECC_GameTraceChannel1;

	FHitResult hitted;
	controller->GetHitResultUnderCursor(traceChannel, false, hitted);

	if (hitted.bBlockingHit)
	{
		TargetComponent->SetTarget(Cast<ACharacter>(hitted.GetActor()));
	}
}

void UCMouseComponent::OnRightClick()
{
	CheckFalse(bShowCursor);

	CheckNotValid(Owner);
	CheckNotValid(TargetComponent);

	TargetComponent->SetTarget(nullptr);
}

void UCMouseComponent::SetMouseOver(AActor* InActor)
{
	CheckTrue(MouseOverActor == InActor);

	if (MouseOverActor.IsValid())
	{
		OffHighlight(MouseOverActor.Get());
	}
	MouseOverActor = InActor;
	OnHighlight(InActor);
}

void UCMouseComponent::OnHighlight(AActor* InActor)
{
	IIHighlightable* highlightable = Cast<IIHighlightable>(MouseOverActor);
	CheckNull(highlightable);

	highlightable->OnHighlight();
}

void UCMouseComponent::OffHighlight(AActor* InActor)
{
	IIHighlightable* highlightable = Cast<IIHighlightable>(MouseOverActor);
	CheckNull(highlightable);

	highlightable->OffHighlight();
}
