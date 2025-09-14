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
	CheckNull(OwnerCharacter);

	Weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	if (!!Weapon)
	{
		Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);
		Weapon->OnActionTypeChanged.AddDynamic(this, &UCAnimInstance::OnActionTypeChanged);
	}

	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	if (!!State)
	{
		State->OnStateTypeChanged.AddDynamic(this, &UCAnimInstance::OnStateTypeChanged);
	}

	Feet = FHelpers::GetComponent<UCFeetComponent>(OwnerCharacter);
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	FRotator rotation = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotation2 = OwnerCharacter->GetControlRotation();
	FRotator result = UKismetMathLibrary::NormalizedDeltaRotator(rotation, rotation2);
	Direction = result.Yaw;

	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();

	bEquip_Sword = WeaponType == EWeaponType::Sword;
	// bKnockdown = StateType == EStateType::Knockdown;
	bDefend = bEquip_Sword && (StateType == EStateType::Defend); // 무기 장착이 된 상태에서 디펜드여야함.

	StateType = State->GetStateType();
	WeaponType = Weapon->GetWeaponType();

	bUseFootIK = false;

	if (!!Feet && !bFalling)
	{
		bUseFootIK = true;
		FeetData = Feet->GetData();
	}

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);

	CheckNull(player);
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
