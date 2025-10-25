#include "Components/CMovementComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "Components/CStateComponent.h"

UCMovementComponent::UCMovementComponent()
{
}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	OnWalk();
	// OwnerCharacter->GetCharacterMovement()->JumpZVelocity = 700;

	OffFixForward();

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	CheckNull(controller);

	controller->PlayerCameraManager->ViewPitchMin = PitchAngle.X;
	controller->PlayerCameraManager->ViewPitchMax = PitchAngle.Y;
}

void UCMovementComponent::OnMoveForward(float InValue)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector	 direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InValue);
}

void UCMovementComponent::OnMoveRight(float InValue)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector	 direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InValue);
}

void UCMovementComponent::OnHorizontalLook(float InValue)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerYawInput(InValue);
}

void UCMovementComponent::OnVerticalLook(float InValue)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InValue);
}

void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::OnJump()
{
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state);
	CheckFalse(state->IsIdleMode());

	OwnerCharacter->Jump();
}

bool UCMovementComponent::IsFalling()
{
	return OwnerCharacter->GetCharacterMovement()->IsFalling();
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnFixForward()
{
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	OwnerCharacter->bUseControllerRotationYaw = true;
}

void UCMovementComponent::OffFixForward()
{
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	OwnerCharacter->bUseControllerRotationYaw = false;
}

void UCMovementComponent::InitMontages()
{
	CheckNull(OwnerCharacter);
}
