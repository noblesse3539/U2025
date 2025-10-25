#include "Weapons/Animations/CAnimInstance_Bow.h"
#include "Global.h"
#include "Weapons/Attachments/CAttachment_Bow.h"
void UCAnimInstance_Bow::NativeBeginPlay()
{
	OwnerBow = Cast<ACAttachment_Bow>(GetOwningActor());
}

void UCAnimInstance_Bow::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	CheckNotValid(OwnerBow);

	DrawBowstring = OwnerBow->GetDrawBowstring();
}
