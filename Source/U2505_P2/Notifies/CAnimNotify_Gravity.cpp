#include "Notifies/CAnimNotify_Gravity.h"

#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCAnimNotify_Gravity::UCAnimNotify_Gravity()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Purple;
#endif
}

FString UCAnimNotify_Gravity::GetNotifyName_Implementation() const
{
	return GetNotifyName("Gravity");
}

void UCAnimNotify_Gravity::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacter* owner = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(owner);
	switch (Type)
	{
		case EAnimNotify_Flow::Begin:
			owner->GetCharacterMovement()->Velocity = FVector::ZeroVector;
			owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
			// owner->GetCharacterMovement()->GravityScale = 0.0f;
			return;
		case EAnimNotify_Flow::End:
			owner->GetCharacterMovement()->Velocity = FVector::ZeroVector;
			owner->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			return;
	}
}