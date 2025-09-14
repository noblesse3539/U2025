#include "Components/CDamagedMontageComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CStateComponent.h"
UCDamagedMontageComponent::UCDamagedMontageComponent()
{
}

void UCDamagedMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);
}

FMontageSet UCDamagedMontageComponent::GetMontageSet()
{
	FMontageSet set;

	CheckNullResult(OwnerCharacter, set);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNullResult(state, set);

	if (state->GetStateType() == EStateType::Knockdown)
	{
		set.Montage = KnockdownMontage;
		set.PlayRate = KnockdownMontage_PlayRate;
		set.bNockdown = true;
	}
	else
	{
		set.Montage = IdleMontage;
		set.PlayRate = IdleMontage_PlayRate;
		set.bNockdown = false;
	}
	return set;
}
