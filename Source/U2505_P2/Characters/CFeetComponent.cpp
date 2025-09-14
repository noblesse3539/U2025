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

// OutDistance : 반대 발이 있는 지면과의 높이차이
void UCFeetComponent::Trace(FName InName, float& OutDistance, FRotator& OutRotation)
{
	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InName);

	// 허리에서 시작
	float	z = OwnerCharacter->GetActorLocation().Z;
	FVector start(location.X, location.Y, z);

	// 허리 - 캡슐절반높이 = 지면. 지면 - TraceDistance = -50정도
	// 이렇게 계산하는 이유는 한 발을 디딜 때 계단 아래일 경우 반대쪽 발을 굽혀야 하기 때문
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
		true, // 지면보다 발이 먼저 닿으므로 복합 충돌을 킴
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
