#include "Squads/CSquad.h"
#include "Global.h"

#include "CGameInstance.h"
#include "ISquadable.h"

#include "CSquadAsset.h"
#include "Characters/CEnemy_AI.h"

void ACSquad::BeginPlay()
{
	Super::BeginPlay();

	if (bPaintRandomColor)
	{
		float r = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
		float g = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
		float b = UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);
		Color = FLinearColor(r, g, b, 1.0f);
	}

	// 게임 인스턴스 내 스쿼드 맵에 스쿼드 등록
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetGameInstance());

	CheckNull(gameInstance);
	CheckFalse(TeamId >= 0 && TeamId < 256);

	CheckTrue(gameInstance->SquadMap.Contains(TeamId)); // 이미 존재하면 생략

	gameInstance->SquadMap.Add(TeamId, this);

	// Asset에서 스쿼드 정보 읽기
	CheckNull(SquadAsset);

	FSquadType squadType = SquadAsset->GetSquadTypes(SquadKey);
	CheckFalse(squadType.SquadMemberCount > 0);

	// 반경 계산 (스폰할 캐릭터 수에 비례)
	float	radius = (SpawnDist * squadType.SquadMemberCount) / (2 * UKismetMathLibrary::GetPI());
	FVector center = GetActorLocation();

	double radian = (2 * UKismetMathLibrary::GetPI()) / squadType.SquadMemberCount;

	// 적 초기화
	for (int32 i = 0; i < squadType.SquadMemberCount; i++)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

		FTransform transform;

		// 회전 값 계산
		float	 degree = UKismetMathLibrary::RadiansToDegrees(radian * i);
		FRotator rotator = FRotator(0.0f, degree, 0.0f);
		transform.SetRotation(FQuat(rotator));

		// 위치 계산
		FVector location = center + radius * rotator.Vector();
		transform.SetLocation(location);

		transform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));

		if (bDebug)
		{
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), location, 50.0f, 16, FLinearColor::Green, 10.0f);
		}
		ACEnemy_AI* ai = GetWorld()->SpawnActorDeferred<ACEnemy_AI>(squadType.EnemyClass, transform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (!IsValid(ai))
			continue;

		ai->SetOwnerSquad(this, (uint8)TeamId);
		ai->SetBodyColor(Color);

		ai->OnDestroyed.AddDynamic(this, &ACSquad::OnDestroyed);

		ai->FinishSpawning(transform);

		Members.AddUnique(ai);
	}
}

void ACSquad::OnOrder(ACharacter* InTarget)
{
	CheckFalse(IsValid(InTarget));

	OrderMembers(InTarget);

	IISquadable* is = Cast<IISquadable>(InTarget);
	if (is == nullptr)
		return;

	ACSquad* targetSquad = is->GetOwnerSquad();
	CheckFalse(IsValid(targetSquad));

	targetSquad->OrderMembers(this);
}

void ACSquad::OrderMembers(ACharacter* InTarget)
{
	CheckFalse(IsValid(InTarget));

	IISquadable* is = Cast<IISquadable>(InTarget);

	// 상대가 Squad가 아닌경우
	if (is == nullptr)
	{
		for (TWeakInterfacePtr<IISquadable> member : Members)
		{
			if (member.IsValid())
				member->SetOrderTarget(InTarget);
		}

		return;
	}

	// 상대가 Squad일 경우 집단전
	ACSquad* targetSquad = is->GetOwnerSquad();

	OrderMembers(targetSquad);
}

void ACSquad::OrderMembers(ACSquad* InTargetSquad)
{
	CheckFalse(IsValid(InTargetSquad));

	int32 minSize = UKismetMathLibrary::Min(Members.Num(), InTargetSquad->Members.Num());

	for (int32 i = 0; i < Members.Num(); i++)
	{
		if (Members[i].IsValid())
		{
			ACharacter* target = Cast<ACharacter>(InTargetSquad->Members[i % minSize].Get());

			Members[i]->SetOrderTarget(target);
		}
	}
}

void ACSquad::OnDestroyed(AActor* DestroyedActor)
{
	IISquadable* squadable = Cast<IISquadable>(DestroyedActor);

	CheckNull(squadable);
	Members.Remove(squadable);
}
