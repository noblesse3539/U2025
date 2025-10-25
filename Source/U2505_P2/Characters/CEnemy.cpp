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

	// Highlight setting
	FHelpers::GetAsset(&HighlightMaterial, "/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_ActorHighlight_Inst.M_ActorHighlight_Inst'");

	GetCapsuleComponent()->SetCapsuleHalfHeight(100);

	TSubclassOf<UCAnimInstance> animInstance;
	FHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/ABP_Character.ABP_Character_C'");
	GetMesh()->SetAnimClass(animInstance);
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(State);
	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	CheckNull(Weapon);
	Weapon->OnWeaponTypeChanged.AddDynamic(this, &ACEnemy::OnWeaponTypeChanged);

	InitOverlayMaterial();

	TArray<UMaterialInterface*> materials = GetMesh()->GetMaterials();
	for (int i = 0; i < materials.Num(); i++)
	{
		FString s = i + " " + materials[i]->GetName();
	}

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
				// StateForDebug->UpdateControllerName(GetController()->GetName()); // TODO Squad 액터를 통한 spawn 시 컨트롤러가 안잡혀서 에러

				if (bDebug)
				{
					State_Widget->SetVisibility(true);
				}
				else
				{
					State_Widget->SetVisibility(false);
				}
			}
		}
	}
}

void ACEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!!State)
		State->OnStateTypeChanged.RemoveDynamic(this, &ACEnemy::OnStateTypeChanged);

	if (!!Weapon)
		Weapon->OnWeaponTypeChanged.RemoveDynamic(this, &ACEnemy::OnWeaponTypeChanged);

	FHelpers::ClearTimer(GetWorld(), Handle_Knockdown);

	Super::EndPlay(EndPlayReason);
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CheckNull(cameraManager);

	// 위젯 방향을 플레이어의 카메라의 정면 방향으로 고정
	if (HP_Widget.IsValid())
	{
		FTransform transform = HP_Widget->GetComponentTransform();

		FVector cameraLocation = cameraManager->GetCameraLocation();

		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(transform.GetLocation(), cameraLocation);
		HP_Widget->SetWorldRotation(rotator);
	}

	if (State_Widget.IsValid())
	{
		FTransform transform = State_Widget->GetComponentTransform();

		FVector cameraLocation = cameraManager->GetCameraLocation();

		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(transform.GetLocation(), cameraLocation);
		State_Widget->SetWorldRotation(rotator);
	}
}

float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CheckNullResult(State, 0);

	if (State->IsDefendMode())
	{
		DamageAmount *= DefenceDamageReduceRate;
	}

	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	TakeDamagedData.Power = damage;
	TakeDamagedData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	TakeDamagedData.AttackCauser = DamageCauser;
	TakeDamagedData.Event = (FDamagedDataEvent*)(&DamageEvent);

	if (State->IsKnockdownMode()) // 넉다운 -> 데미지로 변경안되므로 데미지를 바로 호출
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
		CheckNull(StateForDebug);
		StateForDebug->UpdateState(InNewType);
	}
}

void ACEnemy::Damage()
{
	CheckNull(State);

	if (!!Movement && !State->IsDefendMode())
	{
		Movement->Stop();
	}

	CheckNotValid(HealthPoint);
	// Apply Damage
	HealthPoint->Damage(TakeDamagedData.Power);
	TakeDamagedData.Power = 0.0f;

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
	// GetCapsuleComponent()->SetCollisionProfileName("Dead"); // TODO 바닥이 월드다이나믹인데 무기도 월드다이나믹임.. 해결하기 전까지 사용 X

	if (HP_Widget.IsValid())
	{
		HP_Widget->DestroyComponent();
	}

	if (State_Widget.IsValid())
	{
		State_Widget->DestroyComponent();
	}

	if (!!DeadMontage)
	{
		PlayAnimMontage(DeadMontage, DeadMontage_PlayRate);

		return;
	}

	// 몽타주가 존재하지 않아도 End_Dead를 호출해 객체 소멸
	// FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &ACEnemy::End_Dead);

	FTimerDelegate timerDelegate = FTimerDelegate::CreateWeakLambda(this, [this]() {
		End_Dead();
	});

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, timerDelegate, 0.2f, false);
}

void ACEnemy::Knockdown()
{
	// 넉다운으로 상태 변경 시 이미 땅일 경우 타임 델리게이트 세팅
	UCharacterMovementComponent* movement = GetCharacterMovement();
	CheckNull(movement);

	CheckFalse(movement->IsMovingOnGround());

	UWorld* world = GetWorld();
	CheckNull(world);

	CheckTrue(world->GetTimerManager().IsTimerActive(Handle_Knockdown));

	FTimerDelegate timerDelegate = FTimerDelegate::CreateWeakLambda(this, [this]() {
		End_Knockdown();
	});
	GetWorld()->GetTimerManager().SetTimer(Handle_Knockdown, timerDelegate, KnockdownTime, false);
}

// 피격 몽타주 재생 (디펜드 or 피격)
void ACEnemy::Damaged_PlayMontage()
{
	CheckNull(State);

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

// 피격 시 이펙트 재생
void ACEnemy::Damaged_PlayEffect()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	CheckNull(data);
	FVector effectLocation = GetActorLocation();

	UShapeComponent* collision = TakeDamagedData.Event->Collision;
	if (!!collision)
	{
		FVector point;
		if (collision->GetClosestPointOnCollision(GetActorLocation(), point) > 0.0f)
			effectLocation = point;
	}

	data->PlayEffect(this, effectLocation);
}

// 피격 히트 스탑
void ACEnemy::Damaged_PlayHisStop()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	CheckNull(data);

	if (TakeDamagedData.Event->bFirstHit)
	{
		data->PlayHitStop(this, TakeDamagedData.Attacker);
	}
}

// 피격 카메라 셰이크
void ACEnemy::Damaged_PlayCameraShake()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	CheckNull(data);

	if (TakeDamagedData.Event->bFirstHit)
	{
		data->PlayCameraShake(GetWorld());
	}
}

// 피격 사운드 재생
void ACEnemy::Damaged_PlaySound()
{
	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	CheckNull(data);

	if (TakeDamagedData.Event->bFirstHit)
	{
		data->PlaySound(this);
	}
}

// 피격 시 날아감
void ACEnemy::Damaged_Launch()
{
	CheckNull(State);

	FDamagedData* data = TakeDamagedData.Event->DamagedData;
	CheckNull(data);

	FVector start = GetActorLocation();
	FVector target = TakeDamagedData.Attacker->GetActorLocation();
	FVector direction = start - target;
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

// 피격 시 어태커 방향으로 회전
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
		if (HP_Widget.IsValid())
		{
			HP_Widget->SetVisibility(false);
		}
		if (State_Widget.IsValid())
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

	CheckNull(State);
	State->SetIdleMode();
}

void ACEnemy::End_Knockdown()
{
	CheckNull(State);
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
	CheckNull(State);
	// TODO 추후에 전투중일 때 Idle이 아닐경우도 고려해야함
	State->SetIdleMode();
}

void ACEnemy::Landed(const FHitResult& Hit)
{
	CheckNull(State);
	if (State->IsKnockdownMode())
	{
		CheckTrue(GetWorld()->GetTimerManager().IsTimerActive(Handle_Knockdown));

		FTimerDelegate timerDelegate = FTimerDelegate::CreateWeakLambda(this, [this]() {
			End_Knockdown();
		});
		GetWorld()->GetTimerManager().SetTimer(Handle_Knockdown, timerDelegate, KnockdownTime, false);
	}
}

void ACEnemy::OnHighlight()
{
	CheckNull(HighlightMaterialDynamic);
	HighlightMaterialDynamic->SetScalarParameterValue("LineThickness", 2.0f);
}

void ACEnemy::OffHighlight()
{
	CheckNull(HighlightMaterialDynamic);
	HighlightMaterialDynamic->SetScalarParameterValue("LineThickness", 0.0f);
}

void ACEnemy::InitOverlayMaterial()
{
	HighlightMaterialDynamic = UMaterialInstanceDynamic::Create(HighlightMaterial, this);
	GetMesh()->SetOverlayMaterial(HighlightMaterialDynamic);
	OffHighlight();
}

// Weapon State Type Changed
void ACEnemy::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	CheckNotValid(StateForDebug);

	// FString s = GetName() + " OnWeaponTypeChanged " + FString::FromInt((int8)InPrevType) + " to " + FString::FromInt((int8)InNewType);

	StateForDebug->UpdateWeaponState(InNewType);
}