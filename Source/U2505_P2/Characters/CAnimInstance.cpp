#include "Characters/CAnimInstance.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"

#include "CPlayer.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNotValid(OwnerCharacter);

	Weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter.Get());
	if (Weapon.IsValid())
	{
		Weapon.Get()->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
		Weapon.Get()->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);
	}

	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter.Get());
	if (State.IsValid())
	{
		State.Get()->OnStateTypeChanged.AddDynamic(this, &UCAnimInstance::OnStateTypeChanged);
	}

	Feet = FHelpers::GetComponent<UCFeetComponent>(OwnerCharacter.Get());
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter.IsValid())
	{
		Speed = OwnerCharacter.Get()->GetVelocity().Size2D();

		FRotator rotation = OwnerCharacter.Get()->GetVelocity().ToOrientationRotator();
		FRotator rotation2 = OwnerCharacter.Get()->GetControlRotation();
		FRotator result = UKismetMathLibrary::NormalizedDeltaRotator(rotation, rotation2);
		Direction = result.Yaw;

		bFalling = OwnerCharacter.Get()->GetCharacterMovement()->IsFalling();

		Pitch = -OwnerCharacter->GetBaseAimRotation().Pitch / 90.0f;
	}

	bEquip_Sword = WeaponType == EWeaponType::Sword;
	// bKnockdown = StateType == EStateType::Knockdown;
	bDefend = bEquip_Sword && (StateType == EStateType::Defend); // 무기 장착이 된 상태에서 디펜드여야함.

	if (State.IsValid())
	{
		StateType = State.Get()->GetStateType();
	}

	if (Weapon.IsValid())
	{
		WeaponType = Weapon.Get()->GetWeaponType();

		bBowstring = Weapon.Get()->IsBowstring();
		bPullBowstring = Weapon.Get()->IsPullBowstring();

		if (bPullBowstring)
		{
			Weapon.Get()->GetWeaponSocketLocation("Hand_Bowstring", BowstringLocation);
		}
	}

	bUseFootIK = false;

	if (Feet.IsValid() && !bFalling)
	{
		bUseFootIK = true;
		FeetData = Feet.Get()->GetData();
	}
}

void UCAnimInstance::BeginDestroy()
{
	if (Weapon.IsValid())
	{
		Weapon.Get()->OnWeaponTypeChanged.RemoveDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
		Weapon.Get()->OnActionTypeChanged.RemoveDynamic(this, &UCAnimInstance::OnActionTypeChanged);
	}

	if (State.IsValid())
	{
		State.Get()->OnStateTypeChanged.RemoveDynamic(this, &UCAnimInstance::OnStateTypeChanged);
	}

	Super::BeginDestroy();
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

void UCAnimInstance::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	StateType = InNewType;
}

void UCAnimInstance::OnActionTypeChanged(EActionType InPrevType, EActionType InNewType)
{
	ActionType = InNewType;
}
