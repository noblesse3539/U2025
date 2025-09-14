#include "Notifies/CAnimNotify_State.h"
#include "Global.h"
#include "Characters/ICharacterState.h"
#include "Components/CWeaponComponent.h"

FString UCAnimNotify_State::GetNotifyName_Implementation() const
{
	return "State";
}

void UCAnimNotify_State::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	IICharacterState* state = Cast<IICharacterState>(MeshComp->GetOwner());
	CheckNull(state);

	switch (Type)
	{
		case EStateType::Dead:
			state->End_Dead();
			break;

		case EStateType::Knockdown:
			state->End_Knockdown();
			break;
		case EStateType::Damaged:
			state->End_Damaged();
			break;
		case EStateType::Getup:
			state->End_Getup();
			break;
	}
}