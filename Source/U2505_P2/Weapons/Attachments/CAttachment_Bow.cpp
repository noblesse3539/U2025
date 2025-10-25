#include "Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapons/Animations/CAnimInstance_Bow.h"

ACAttachment_Bow::ACAttachment_Bow()
{
	FHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);

	USkeletalMesh* mesh;
	FHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Meshes/Bow/SK_Bow.SK_Bow'");
	SkeletalMesh->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance_Bow> animInstance;
	FHelpers::GetClass<UCAnimInstance_Bow>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/Weapons/Bow/ABP_Bow.ABP_Bow_C'");
	SkeletalMesh->SetAnimInstanceClass(animInstance);
}
