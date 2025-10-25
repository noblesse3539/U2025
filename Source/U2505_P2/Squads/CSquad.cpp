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

	// ���� �ν��Ͻ� �� ������ �ʿ� ������ ���
	UCGameInstance* gameInstance = Cast<UCGameInstance>(GetGameInstance());

	CheckNull(gameInstance);
	CheckFalse(TeamId >= 0 && TeamId < 256);

	CheckTrue(gameInstance->SquadMap.Contains(TeamId)); // �̹� �����ϸ� ����

	gameInstance->SquadMap.Add(TeamId, this);

	// Asset���� ������ ���� �б�
	CheckNull(SquadAsset);

	FSquadType squadType = SquadAsset->GetSquadTypes(SquadKey);
	CheckFalse(squadType.SquadMemberCount > 0);

	// �ݰ� ��� (������ ĳ���� ���� ���)
	float	radius = (SpawnDist * squadType.SquadMemberCount) / (2 * UKismetMathLibrary::GetPI());
	FVector center = GetActorLocation();

	double radian = (2 * UKismetMathLibrary::GetPI()) / squadType.SquadMemberCount;

	// �� �ʱ�ȭ
	for (int32 i = 0; i < squadType.SquadMemberCount; i++)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		params.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

		FTransform transform;

		// ȸ�� �� ���
		float	 degree = UKismetMathLibrary::RadiansToDegrees(radian * i);
		FRotator rotator = FRotator(0.0f, degree, 0.0f);
		transform.SetRotation(FQuat(rotator));

		// ��ġ ���
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

	// ��밡 Squad�� �ƴѰ��
	if (is == nullptr)
	{
		for (TWeakInterfacePtr<IISquadable> member : Members)
		{
			if (member.IsValid())
				member->SetOrderTarget(InTarget);
		}

		return;
	}

	// ��밡 Squad�� ��� ������
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
