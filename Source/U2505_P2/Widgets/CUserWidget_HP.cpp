#include "Widgets/CUserWidget_HP.h"
#include "Global.h"
#include "GameFramework/PlayerController.h"

void UCUserWidget_HP::NativeOnInitialized()
{
	HealthPoint = MaxHealthPoint;
	HealthTail = HealthPoint;
	StaminaPoint = 0.0f;

	Super::NativeOnInitialized();
}

void UCUserWidget_HP::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TickHealthTail(InDeltaTime);
	TickStamina(InDeltaTime);
}

float UCUserWidget_HP::GetHealthPointRatio()
{
	return HealthPoint / MaxHealthPoint;
}

float UCUserWidget_HP::GetHealthTailRatio()
{
	return HealthTail / MaxHealthPoint;
}

float UCUserWidget_HP::GetStaminaPointRatio()
{
	return StaminaPoint / MaxStaminaPoint;
}

void UCUserWidget_HP::Damage(float InAmount)
{
	HealthPoint -= InAmount;
	HealthPoint = FMath::Clamp<float>(HealthPoint, 0, MaxHealthPoint);

	UpdateHealthPoint();

	APlayerController* player = GetOwningPlayer();
	CheckNull(player);
	CheckTrue(bCanUpdateHealthTail);

	FTimerDelegate healthTaileTimer = FTimerDelegate::CreateUObject(this, &UCUserWidget_HP::OnCanUpdateHealthTail);
	player->GetWorld()->GetTimerManager().SetTimer(Handle_healthTail, healthTaileTimer, Delay_TailTimer, false);
}

void UCUserWidget_HP::DecreaseStamina(float InAmount)
{
	StaminaPoint -= InAmount;
	StaminaPoint = FMath::Clamp<float>(StaminaPoint, 0, MaxStaminaPoint);

	UpdateStaminaPoint();

	APlayerController* player = GetOwningPlayer();
	CheckNull(player);

	OffCanRecovery();
	FTimerDelegate recoveryTimer = FTimerDelegate::CreateUObject(this, &UCUserWidget_HP::OnCanRecovery);
	player->GetWorld()->GetTimerManager().SetTimer(Handle_Recovery, recoveryTimer, Delay_RecoveryTimer, false);
}

void UCUserWidget_HP::TickHealthTail(float InDeltaTime)
{
	CheckFalse(bCanUpdateHealthTail);

	if (abs(HealthTail - HealthPoint) <= 10e-6)
	{
		OffCanUpdateHealthTail();
		return;
	}

	if (HealthTail < HealthPoint)
	{
		HealthTail = HealthPoint;
		UpdateHealthTail();
		return;
	}

	HealthTail = UKismetMathLibrary::FInterpTo(HealthTail, HealthPoint, InDeltaTime, RecoveryInterpSpeed);
	UpdateHealthTail();
}

void UCUserWidget_HP::TickStamina(float InDeltaTime)
{
	CheckTrue(StaminaPoint >= MaxStaminaPoint);

	if (bCanRecovery)
	{
		StaminaPoint = UKismetMathLibrary::FInterpTo(StaminaPoint, MaxStaminaPoint, InDeltaTime, RecoveryInterpSpeed);
	}
	else
	{
		StaminaPoint += InDeltaTime * RecoverySpeed;
	}
	StaminaPoint = FMath::Clamp<float>(StaminaPoint, 0, MaxStaminaPoint);

	UpdateStaminaPoint();
}
