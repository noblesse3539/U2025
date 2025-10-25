#include "Characters/CPlayer.h"
#include "Global.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"

#include "CAnimInstance.h"
#include "Widgets/CUserWidget_HP.h"

#include "CZoomComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMouseComponent.h"
#include "Components/CTargetComponent.h"

#include "Weapons/Actions/CAction_Defend.h"

ACPlayer::ACPlayer()
{
	FHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	FHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	FHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm2, "SpringArm2", GetCapsuleComponent());
	FHelpers::CreateComponent<UCameraComponent>(this, &Camera2, "Camera2", SpringArm2);

	FHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	FHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	FHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	FHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");
	FHelpers::CreateActorComponent<UCMouseComponent>(this, &Mouse, "Mouse");
	FHelpers::CreateActorComponent<UCTargetComponent>(this, &TargetComponent, "TargetComponent");

	USkeletalMesh* mesh;
	FHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/FlexibleCombatSystem/DemoFolder/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UCAnimInstance> animInstance;
	FHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);

	// Main Camera
	{
		Camera->SetRelativeLocation(FVector(0, 50, 0));
		SpringArm->SetRelativeLocation(FVector(0, 0, 60));
		SpringArm->TargetArmLength = 420;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bEnableCameraLag = true;
	}

	// Sub Camera
	{
		Camera2->SetRelativeLocation(FVector(0, 50, 0));
		SpringArm2->SetRelativeLocation(FVector(0, 0, 60));
		SpringArm2->SetRelativeRotation(FRotator(0, -90, 0));
		SpringArm->TargetArmLength = 420;
		SpringArm2->bUsePawnControlRotation = true;
		SpringArm2->bEnableCameraLag = true;
	}
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);

	HP_Widget = FHelpers::GetComponent<UWidgetComponent>(this);
	CheckNull(HP_Widget);

	UUserWidget* widget = HP_Widget->GetUserWidgetObject();
	widget->AddToViewport();

	HealthPoint = Cast<UCUserWidget_HP>(widget);
}

void ACPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!!State)
		State->OnStateTypeChanged.RemoveDynamic(this, &ACPlayer::OnStateTypeChanged);

	// if (!!Weapon)
	// Weapon->OnWeaponTypeChanged.RemoveDynamic(this, &ACPlayer::OnWeaponTypeChanged);

	Super::EndPlay(EndPlayReason);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnWalk);
	PlayerInputComponent->BindAction("Jumping", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnJump);

	PlayerInputComponent->BindAction("Katana", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetKatanaMode);
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
	// PlayerInputComponent->BindAction("Axe", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetAxeMode);
	PlayerInputComponent->BindAction("Magic", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetMagicMode);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);

	PlayerInputComponent->BindAction("Action2", EInputEvent::IE_Pressed, this, &ACPlayer::Action2_Pressed);
	PlayerInputComponent->BindAction("Action2", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::Action_Defend_Released);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::Action_Pressed);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Released, Weapon, &UCWeaponComponent::OffCharge); // Todo

	PlayerInputComponent->BindAction("Action3", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Action_Air_Pressed);
	PlayerInputComponent->BindAction("Action4", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Action_Finish_Pressed); // Todo

	PlayerInputComponent->BindAction("Skill1", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::Action_Skill_Pressed); // Todo

	PlayerInputComponent->BindAction("ShowCursor", EInputEvent::IE_Pressed, Mouse, &UCMouseComponent::ShowCursor);
	PlayerInputComponent->BindAction("ShowCursor", EInputEvent::IE_Released, Mouse, &UCMouseComponent::HideCursor);

	if (!!Zoom)
		PlayerInputComponent->BindAxis("Zoom", Zoom, &UCZoomComponent::SetValue);
}

void ACPlayer::Action_Pressed()
{
	APlayerController* controller = GetController<APlayerController>();
	CheckNull(controller);

	if (controller->IsInputKeyDown(EKeys::LeftAlt))
	{
		CheckNull(Mouse);
		Mouse->OnLeftClick();
	}
	else
	{
		CheckNull(Weapon);
		Weapon->DoAction_Attack();
	}
}

void ACPlayer::Action2_Pressed()
{
	APlayerController* controller = GetController<APlayerController>();
	CheckNull(controller);

	if (controller->IsInputKeyDown(EKeys::LeftAlt))
	{
		CheckNull(Mouse);
		Mouse->OnRightClick();
	}
	else
	{
		CheckNull(Weapon);
		Weapon->Action_Defend_Pressed();
	}
}

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (State->IsDefendMode())
	{
		DamageAmount *= DefenceDamageReduceRate;
	}

	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	TakeDamagedData.Power = damage;
	TakeDamagedData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	TakeDamagedData.AttackCauser = DamageCauser;
	TakeDamagedData.Event = (FDamagedDataEvent*)(&DamageEvent);

	if (State->IsKnockdownMode()) // �˴ٿ� -> �������� ����ȵǹǷ� �������� �ٷ� ȣ��
	{
		Damage();
	}
	else if (State->IsDefendMode())
	{
		Damage();
	}
	else
	{
		State->SetDamagedMode();
	}

	return damage;
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Damaged:
			Damage();
			break;
		case EStateType::Dead:
			Dead();
			break;
		case EStateType::Knockdown:
			Knockdown();
			break;
	}
}

void ACPlayer::Damage()
{
	CheckNull(State);

	// ������ ����
	if (!!Movement && !State->IsDefendMode())
	{
		Movement->Stop();
	}

	// Apply Damage
	if (!!HealthPoint)
	{
		HealthPoint->Damage(TakeDamagedData.Power);
		TakeDamagedData.Power = 0.0f;
	}

	if (!!TakeDamagedData.Event && !!TakeDamagedData.Event->DamagedData)
	{

		Damaged_PlayMontage();

		Damaged_PlayEffect();

		Damaged_PlayHisStop();
		Damaged_PlayCameraShake();
		Damaged_PlaySound();

		if (HealthPoint->IsDead() == false)
		{
			Damaged_Launch();

			Damaged_LookAt();

			FDamagedData* data = TakeDamagedData.Event->DamagedData;
			if (data->bKnockdown)
			{
				State->SetKnockdownMode();
			}
		}
	}

	if (HealthPoint->IsDead())
	{
		State->SetDeadMode();
	}

	TakeDamagedData.Attacker = nullptr;
	TakeDamagedData.AttackCauser = nullptr;
	TakeDamagedData.Event = nullptr;
}

void ACPlayer::Dead()
{
	// if (!!HP_Widget)
	//	HP_Widget->DestroyComponent();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!!DeadMontage)
	{
		PlayAnimMontage(DeadMontage, DeadMontage_PlayRate);

		return;
	}

	// ��Ÿ�ְ� �������� �ʾƵ� End_Dead�� ȣ���� ��ü �Ҹ�

	FTimerDelegate timerDelegate = FTimerDelegate::CreateWeakLambda(this, [this]() {
		End_Dead();
	});

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, timerDelegate, 0.2f, false);
}

void ACPlayer::Knockdown()
{
}

// �ǰ� ��Ÿ�� ��� (����� or �ǰ�)
void ACPlayer::Damaged_PlayMontage()
{
	if (State->IsDefendMode())
	{
		CheckNull(Weapon);
		UCAction_Defend* defend = Weapon->GetAction_Defend();
		CheckNull(defend);
		CheckNull(defend->DefendMontage);

		PlayAnimMontage(defend->DefendMontage, defend->DefendMontage_PlayRate);

		return;
	}

	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	CheckNull(data);
	data->PlayHitMotion(this);
}

// �ǰ� �� ����Ʈ ���
void ACPlayer::Damaged_PlayEffect()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	FVector		  effectLocation = GetActorLocation();

	UShapeComponent* collision = TakeDamagedData.Event->Collision;
	if (!!collision)
	{
		FVector point;
		if (collision->GetClosestPointOnCollision(GetActorLocation(), point) > 0.0f)
			effectLocation = point;
	}

	data->PlayEffect(this, effectLocation);
}

// �ǰ� ��Ʈ ��ž
void ACPlayer::Damaged_PlayHisStop()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	if (TakeDamagedData.Event->bFirstHit)
	{
		data->PlayHitStop(this, TakeDamagedData.Attacker);
	}
}

// �ǰ� ī�޶� ����ũ
void ACPlayer::Damaged_PlayCameraShake()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	if (TakeDamagedData.Event->bFirstHit)
	{
		data->PlayCameraShake(GetWorld());
	}
}

// �ǰ� ���� ���
void ACPlayer::Damaged_PlaySound()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	if (TakeDamagedData.Event->bFirstHit)
	{
		data->PlaySound(this);
	}
}

// �ǰ� �� ���ư�
void ACPlayer::Damaged_Launch()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	FVector		  start = GetActorLocation();
	FVector		  target = TakeDamagedData.Attacker->GetActorLocation();
	FVector		  direction = start - target;
	direction.Normalize();

	if (!State->IsDefendMode())
	{
		switch (data->StateType)
		{
			case EDamagedStateType::Normal:
			{
				// GetCharacterMovement()->SetMovementMode(MOVE_Falling);
				if (State->IsKnockdownMode())
				{
					LaunchCharacter(FVector(data->LaunchXZ.X * 0.01f, 0, AirHitZVelocity), false, true);
				}
				else
				{
					LaunchCharacter(direction * data->LaunchXZ.X, false, false);
				}
				break;
			}
			case EDamagedStateType::Flying:
			{
				// GetCharacterMovement()->SetMovementMode(MOVE_Flying);

				if (State->IsKnockdownMode())
				{
					LaunchCharacter(FVector(0, 0, AirHitZVelocity), false, true);
				}
				else
				{
					LaunchCharacter(FVector(0, 0, 1) * data->LaunchXZ.Y, false, true);
				}
				break;
			}
			case EDamagedStateType::GroundPound:
			{
				// GetCharacterMovement()->SetMovementMode(MOVE_Falling);
				LaunchCharacter(FVector(0, 0, -1) * data->LaunchXZ.Y, false, true);
				break;
			}
		}
	}
}

// �ǰ� �� ����Ŀ �������� ȸ��
void ACPlayer::Damaged_LookAt()
{
	FVector start = GetActorLocation();
	FVector target = TakeDamagedData.Attacker->GetActorLocation();
	target.Z = start.Z;
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
}

void ACPlayer::End_Dead()
{
	TArray<AActor*> actors;
	GetAttachedActors(actors, true, true);

	for (int32 i = actors.Num() - 1; i >= 0; i--)
	{
		actors[i]->Destroy();
	}

	Destroy();
}

void ACPlayer::End_Damaged()
{
	Movement->Move();
	State->SetIdleMode();
}

void ACPlayer::End_Knockdown()
{
	if (!!GetupMontage)
	{
		PlayAnimMontage(GetupMontage, GetupMontage_PlayRate);
	}
	else
	{
		State->SetIdleMode();
	}
}

void ACPlayer::End_Getup()
{
	// TODO ���Ŀ� �������� �� Idle�� �ƴҰ�쵵 ����ؾ���
	State->SetIdleMode();
}

void ACPlayer::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	TeamID = InTeamID;
}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return TeamID;
}

void ACPlayer::Landed(const FHitResult& Hit)
{
	if (State->IsActionMode() && Weapon->GetActionType() == EActionType::DownStrike)
	{
		Weapon->Begin_DownStrike();
	}
}

bool ACPlayer::IsPosessable()
{
	return PossessCnt > 0;
}

void ACPlayer::Possess()
{
	PossessCnt = UKismetMathLibrary::Clamp(PossessCnt - 1, 0, 2);
}

void ACPlayer::DisPossess()
{
	PossessCnt = UKismetMathLibrary::Clamp(PossessCnt + 1, 0, 2);
}

void ACPlayer::ChangeCamera(bool bOrigin)
{
	if (bOrigin)
	{
		Camera->Activate();
		Camera2->Deactivate();
	}
	else
	{
		Camera->Deactivate();

		float	z = Camera->GetComponentLocation().Z;
		FVector location = Camera2->GetComponentLocation();
		location.Z = z;
		Camera2->SetWorldLocation(location);

		float	 pitch = Camera->GetComponentRotation().Pitch;
		FRotator rotation = Camera2->GetComponentRotation();
		rotation.Pitch = pitch;
		Camera2->SetWorldRotation(rotation);
		Camera2->Activate();
	}
}

void ACPlayer::DecreaseStamina(float InAmount)
{
	CheckNull(HealthPoint);
	HealthPoint->DecreaseStamina(InAmount);
}
