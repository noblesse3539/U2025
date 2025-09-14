#include "Characters/CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

// #define LOG_UCFeetComponent

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float	 leftDistance;
	FRotator leftRotation;
	Trace(LeftBoneName, leftDistance, leftRotation);

	float	 rightDistance;
	FRotator rightRotation;
	Trace(RightBoneName, rightDistance, rightRotation);

	float pelvisDistance = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, pelvisDistance, DeltaTime, InterpSpeed);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - pelvisDistance), DeltaTime, InterpSpeed);

	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - pelvisDistance), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);

#ifdef LOG_UCFeetComponent
	FLog::Print(Data.LeftDistance, 9990, 10, FColor::Yellow);
	FLog::Print(Data.RightDistance, 9980, 10, FColor::Yellow);
	FLog::Print(Data.PelvisDistance, 9970, 10, FColor::Yellow);
	FLog::Print(Data.LeftRotation, 9960, 10, FColor::Yellow);
	FLog::Print(Data.RightRotation, 9950, 10, FColor::Yellow);
#endif
}

// OutDistance : �ݴ� ���� �ִ� ������� ��������
void UCFeetComponent::Trace(FName InName, float& OutDistance, FRotator& OutRotation)
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InName);

	// �㸮���� ����
	float	z = OwnerCharacter->GetActorLocation().Z;
	FVector start(location.X, location.Y, z);

	// �㸮 - ĸ�����ݳ��� = ����. ���� - TraceDistance = -50����
	// �̷��� ����ϴ� ������ �� ���� ��� �� ��� �Ʒ��� ��� �ݴ��� ���� ������ �ϱ� ����
	z = z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector end(location.X, location.Y, z);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	EDrawDebugTrace::Type type = bDebug ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	FHitResult hitResult;

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		start,
		end,
		ETraceTypeQuery::TraceTypeQuery1,
		true, // ���麸�� ���� ���� �����Ƿ� ���� �浹�� Ŵ
		ignores,
		type,
		hitResult,
		true);

	OutDistance = 0.0;
	OutRotation = FRotator::ZeroRotator;

	CheckFalse(hitResult.bBlockingHit);

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = length + OffsetDistance - TraceDistance;

	float roll = UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z);

	OutRotation = FRotator(pitch, 0.0f, roll);
}
