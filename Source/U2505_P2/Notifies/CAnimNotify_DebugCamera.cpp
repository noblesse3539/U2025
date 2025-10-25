#include "Notifies/CAnimNotify_DebugCamera.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CWeaponComponent.h"
#include "Characters/CPlayer.h"
#include "Camera/CameraComponent.h"

UCAnimNotify_DebugCamera::UCAnimNotify_DebugCamera()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(20, 150, 230);
#endif
}

FString UCAnimNotify_DebugCamera::GetNotifyName_Implementation() const
{
	return GetNotifyName("Debug Camera");
}

void UCAnimNotify_DebugCamera::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(player);

	UCWeaponComponent* weapon = GetWeaponComponent(MeshComp);
	CheckNull(weapon);

	CheckFalse(weapon->IsDebugCamera());

	APlayerController* controller = player->GetWorld()->GetFirstPlayerController();
	CheckNull(controller);

	TArray<UCameraComponent*> cameras;
	switch (Type)
	{
		case EAnimNotify_Flow::Begin:
			player->ChangeCamera(false);
			return;

		case EAnimNotify_Flow::End:
			player->ChangeCamera(true);
			return;
	}
}
