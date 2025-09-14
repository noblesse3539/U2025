#include "Characters/CEnemy.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "CAnimInstance.h"
#include "Widgets/CUserWidget_HP.h"
#include "Widgets/CUserWidget_State.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CDamagedMontageComponent.h"

#include "Weapons/CWeaponStructures.h"
#include "Weapons/Actions/CAction_Defend.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	FHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	FHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	FHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	// FHelpers::CreateActorComponent<UCDamagedMontageComponent>(this, &DamagedMontageCmp, "DamagedMontageCmp");

	USkeletalMesh* mesh;
	FHelpers::GetAsset(&mesh, "/Script/Engine.SkeletalMesh'/Game/Characters/Enemies/Orc_Pack/Orc/Mesh/Separate/SK_Orc_Body.SK_Orc_Body'");
	GetMesh()->SetSkeletalMesh(mesh);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -100));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));

	GetCapsuleComponent()->SetCapsuleHalfHeight(100);

	TSubclassOf<UCAnimInstance> animInstance;
	FHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);
}

void ACEnemy::BeginPlay()
{
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();

	StartLocation = GetActorLocation();

	TArray<UWidgetComponent*> widgetComponents;
	GetComponents(widgetComponents);

	for (UWidgetComponent* widget : widgetComponents)
	{

		UUserWidget* userWidget = widget->GetUserWidgetObject();

		// HP Widget
		{
			UCUserWidget_HP* temp = Cast<UCUserWidget_HP>(userWidget);
			if (!!temp)
			{
				HP_Widget = widget;
				HealthPoint = temp;
			}
		}

		// State Widget
		{
			UCUserWidget_State* temp = Cast<UCUserWidget_State>(userWidget);
			if (!!temp)
			{
				State_Widget = widget;
				StateForDebug = temp;

				StateForDebug->UpdateName(GetName());
				// StateForDebug->UpdateControllerName(GetController()->GetName()); // TODO Squad ���͸� ���� spawn �� ��Ʈ�ѷ��� �������� ����

				if (bDebug)
				{
					State_Widget->SetVisibility(true);
				}
				else
				{
					State_Widget->SetVisibility(false);
				}

				CheckNull(Weapon);
				Weapon->OnWeaponTypeChanged.AddDynamic(this, &ACEnemy::OnWeaponTypeChanged);
			}
		}
	}
	// HP_Widget = FHelpers::GetComponent<UWidgetComponent>(this);
	// CheckNull(HP_Widget);

	// UUserWidget* widget = HP_Widget->GetUserWidgetObject();
	// HealthPoint = Cast<UCUserWidget_HP>(widget);

	// HP_Widget = FHelpers::GetComponent<UWidgetComponent>(this);
	// CheckNull(HP_Widget);

	// UUserWidget* widget = HP_Widget->GetUserWidgetObject();
	// HealthPoint = Cast<UCUserWidget_HP>(widget);
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� ������ �÷��̾��� ī�޶��� ���� �������� ����
	if (IsValid(HP_Widget))
	{
		FTransform transform = HP_Widget->GetComponentTransform();

		APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		FVector				  cameraLocation = cameraManager->GetTransformComponent()->GetComponentLocation();

		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(transform.GetLocation(), cameraLocation);
		HP_Widget->SetWorldRotation(rotator);
	}

	if (IsValid(State_Widget))
	{
		FTransform transform = State_Widget->GetComponentTransform();

		APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		FVector				  cameraLocation = cameraManager->GetTransformComponent()->GetComponentLocation();

		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(transform.GetLocation(), cameraLocation);
		State_Widget->SetWorldRotation(rotator);
	}
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void ACEnemy::SetDeadMontage(UAnimMontage* InMontage, float InPlayRate)
{
	DeadMontage = InMontage;
	DeadMontage_PlayRate = InPlayRate;
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
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

	if (bDebug)
	{
		StateForDebug->UpdateState(InNewType);
	}
}

void ACEnemy::Damage()
{
	if (!!Movement)
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

void ACEnemy::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// GetCapsuleComponent()->SetCollisionProfileName("Dead"); // TODO �ٴ��� ������̳����ε� ���⵵ ������̳�����.. �ذ��ϱ� ������ ��� X

	if (!!HP_Widget)
	{
		HP_Widget->DestroyComponent();
		HP_Widget = nullptr;
	}

	if (!!State_Widget)
	{
		State_Widget->DestroyComponent();
		State_Widget = nullptr;
	}

	if (!!DeadMontage)
	{
		PlayAnimMontage(DeadMontage, DeadMontage_PlayRate);

		return;
	}

	// ��Ÿ�ְ� �������� �ʾƵ� End_Dead�� ȣ���� ��ü �Ҹ�
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &ACEnemy::End_Dead);

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, timerDelegate, 0.2f, false);
}

void ACEnemy::Knockdown()
{
	// �˴ٿ����� ���� ���� �� �̹� ���� ��� Ÿ�� ��������Ʈ ����
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &ACEnemy::End_Knockdown);
		if (!GetWorld()->GetTimerManager().IsTimerActive(Handle_Knockdown))
		{
			GetWorld()->GetTimerManager().SetTimer(Handle_Knockdown, timerDelegate, KnockdownTime, false);
		}
	}
}

// �ǰ� ��Ÿ�� ��� (����� or �ǰ�)
void ACEnemy::Damaged_PlayMontage()
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
void ACEnemy::Damaged_PlayEffect()
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
void ACEnemy::Damaged_PlayHisStop()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	if (TakeDamagedData.Event->bFirstHit)
	{
		data->PlayHitStop(this, TakeDamagedData.Attacker);
	}
}

// �ǰ� ī�޶� ����ũ
void ACEnemy::Damaged_PlayCameraShake()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	if (TakeDamagedData.Event->bFirstHit)
	{
		data->PlayCameraShake(GetWorld());
	}
}

// �ǰ� ���� ���
void ACEnemy::Damaged_PlaySound()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	if (TakeDamagedData.Event->bFirstHit)
	{
		data->PlaySound(this);
	}
}

// �ǰ� �� ���ư�
void ACEnemy::Damaged_Launch()
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
void ACEnemy::Damaged_LookAt()
{
	FVector start = GetActorLocation();
	FVector target = TakeDamagedData.Attacker->GetActorLocation();
	target.Z = start.Z;
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
}

void ACEnemy::End_Dead()
{
	TArray<AActor*> actors;
	GetAttachedActors(actors, true, true);

	for (int32 i = actors.Num() - 1; i >= 0; i--)
	{
		actors[i]->Destroy();
	}

	SetActorTickEnabled(false);
	{
		if (IsValid(HP_Widget))
		{
			HP_Widget->SetVisibility(false);
		}
		if (IsValid(State_Widget))
		{
			State_Widget->SetVisibility(false);
		}
	}
	HP_Widget = nullptr;
	State_Widget = nullptr;

	Destroy();
}

void ACEnemy::End_Damaged()
{
	if (!!Movement)
	{
		Movement->Move();
	}

	State->SetIdleMode();
}

void ACEnemy::End_Knockdown()
{
	CheckTrue(State->IsDeadMode());

	if (!!GetupMontage)
	{
		PlayAnimMontage(GetupMontage, GetupMontage_PlayRate);
	}
	else
	{
		State->SetIdleMode();
	}
}

void ACEnemy::End_Getup()
{
	// TODO ���Ŀ� �������� �� Idle�� �ƴҰ�쵵 ����ؾ���
	State->SetIdleMode();
}

void ACEnemy::Landed(const FHitResult& Hit)
{
	if (State->IsKnockdownMode())
	{
		FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &ACEnemy::End_Knockdown);
		if (!GetWorld()->GetTimerManager().IsTimerActive(Handle_Knockdown))
		{
			GetWorld()->GetTimerManager().SetTimer(Handle_Knockdown, timerDelegate, KnockdownTime, false);
		}
	}
}

// Weapon State Type Changed
void ACEnemy::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	CheckNull(StateForDebug);

	FString s = GetName() + " OnWeaponTypeChanged " + FString::FromInt((int8)InPrevType) + " to " + FString::FromInt((int8)InNewType);

	StateForDebug->UpdateWeaponState(InNewType);
}