#include "Weapons/Etcs/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACGhostTrail::ACGhostTrail()
{
	PrimaryActorTick.bCanEverTick = true;

	FHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");
	FHelpers::CreateComponent<USceneComponent>(this, &Meshes, "Meshes");
}

void ACGhostTrail::SetMaterial(const FLinearColor& InColor, float InExponent)
{
	CreateAndSetMaterial();

	Material->SetVectorParameterValue("Color", InColor);
	Material->SetScalarParameterValue("Exponent", InExponent);
}

void ACGhostTrail::SetMeshScale(const FVector& InScale)
{
	Mesh->SetRelativeScale3D(InScale);
}

void ACGhostTrail::SetInterval(float InValue)
{
	Interval = InValue;
}

void ACGhostTrail::SetCaptureCount(int32 InValue)
{
	CaptureCount = InValue;
}

void ACGhostTrail::SetHasChildrenMeshes(bool InValue)
{
	bHasChildren = InValue;
}

void ACGhostTrail::SetUseInterp(bool bInUseInterp)
{
	bUseInterp = bInUseInterp;
}

void ACGhostTrail::SetInterpStartInterval(float InInterpStartInterval)
{
	InterpStartInterval = InInterpStartInterval;
}

void ACGhostTrail::SetInterpExponent(float InInterpExponent)
{
	InterpExponent = InInterpExponent;
}

void ACGhostTrail::SetDuration(float InDuration)
{
	Duration = InDuration;
}

void ACGhostTrail::MarkedDestroy()
{
	if (CaptureCount < 1)
	{
		Destroy();

		return;
	}

	DestroyIndex = CurrentDestroyIndex = MeshesCount;
}

void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	bUseInterp ? SetActorTickInterval(InterpStartInterval) : SetActorTickInterval(Interval);

	CreateAndSetMaterial();

	USkeletalMeshComponent* ownerMesh = OwnerCharacter->GetMesh();
	CheckNull(ownerMesh);

	for (int32 i = 0; i < ownerMesh->GetMaterials().Num(); i++)
	{
		Mesh->SetMaterial(i, Material);
	}

	if (CaptureCount < 1)
	{
		Mesh->SetSkinnedAssetAndUpdate(ownerMesh->GetSkeletalMeshAsset());
		Mesh->CopyPoseFromSkeletalComponent(ownerMesh);
	}
	else
	{
		for (int32 i = 0; i < CaptureCount; i++)
		{
			FPoseableMeshParts poseableMeshParts;
			for (USceneComponent* child : ownerMesh->GetAttachChildren())
			{
				USkeletalMeshComponent* childMesh = Cast<USkeletalMeshComponent>(child);
				if (childMesh == nullptr)
					continue;

				if (childMesh->GetName().Equals("Cloak"))
					continue;

				FString					name = FString::Printf(L"Meshes%d", i + 1) + childMesh->GetName();
				UPoseableMeshComponent* mesh = NewObject<UPoseableMeshComponent>(this, UPoseableMeshComponent::StaticClass(), FName(name));

				if (!!mesh)
				{
					mesh->RegisterComponent();
					mesh->AttachToComponent(Meshes, FAttachmentTransformRules::KeepRelativeTransform);

					for (int32 k = 0; k < childMesh->GetMaterials().Num(); k++)
					{
						mesh->SetMaterial(k, Material);
					}

					mesh->SetSkinnedAssetAndUpdate(childMesh->GetSkeletalMeshAsset());

					poseableMeshParts.Meshs.Add(mesh);
				}
			}
			CopyPose(poseableMeshParts);

			PoseableMeshes.Add(poseableMeshParts);
		}
	}
}

void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ACGhostTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 보간 적용할 경우 틱 인터벌을 보간
	if (bUseInterp)
	{
		InterpAlpha += DeltaTime;
		float nextTickInterval = UKismetMathLibrary::FInterpEaseInOut(InterpStartInterval, Interval, InterpAlpha / Duration, InterpExponent);
		SetActorTickInterval(nextTickInterval);
	}

	if (DestroyIndex < 0)
	{
		if (CaptureCount < 1)
		{
			CopyPose(PoseableMeshes[MeshesCount]);

			return;
		}

		MeshesCount++;
		MeshesCount %= PoseableMeshes.Num();

		CopyPose(PoseableMeshes[MeshesCount]);

		return;
	}

	CurrentDestroyIndex++;
	CurrentDestroyIndex %= CaptureCount;

	for (UPoseableMeshComponent* mesh : PoseableMeshes[CurrentDestroyIndex].Meshs)
	{
		mesh->DestroyComponent();
	}

	if (CurrentDestroyIndex == DestroyIndex)
		Destroy();
}

void ACGhostTrail::CreateAndSetMaterial()
{
	if (!!Material)
		return;

	UMaterialInstanceConstant* material;
	FHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");

	Material = UMaterialInstanceDynamic::Create(material, this);
}

void ACGhostTrail::CopyPose(FPoseableMeshParts& InMeshParts)
{
	CheckTrue(InMeshParts.Meshs.Num() == 0);

	FVector	 location = OwnerCharacter->GetMesh()->GetComponentLocation();
	FRotator rotator = OwnerCharacter->GetMesh()->GetComponentRotation();

	for (UPoseableMeshComponent* mesh : InMeshParts.Meshs)
	{
		mesh->SetRelativeLocation(location);
		mesh->SetRelativeRotation(rotator);

		mesh->CopyPoseFromSkeletalComponent(OwnerCharacter->GetMesh());
	}
}
