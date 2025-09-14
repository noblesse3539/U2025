#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CGhostTrail.generated.h"

USTRUCT()
struct FPoseableMeshParts
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<class UPoseableMeshComponent*> Meshs;
};

UCLASS()
class U2505_P2_API ACGhostTrail : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class UPoseableMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UPoseableMeshComponent* ChildMeshes;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Meshes;

public:
	ACGhostTrail();

public:
	void SetMaterial(const FLinearColor& InColor, float InExponent);
	void SetMeshScale(const FVector& InScale);
	void SetInterval(float InValue);
	void SetCaptureCount(int32 InValue);
	void SetHasChildrenMeshes(bool InValue);
	void SetUseInterp(bool bInUseInterp);
	void SetInterpStartInterval(float InInterpStartInterval);
	void SetInterpExponent(float InInterpExponent = 1.0f);
	void SetDuration(float InDuration);

	void MarkedDestroy();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	class ACharacter*				OwnerCharacter;
	class UMaterialInstanceDynamic* Material = nullptr;

private:
	void CreateAndSetMaterial();
	void CopyPose(FPoseableMeshParts& InMeshParts);

private:
	bool bHasChildren;

private:
	float Interval;

private:
	int32 CaptureCount;

private:
	UPROPERTY()
	TArray<FPoseableMeshParts> PoseableMeshes;
	int32					   MeshesCount;

private:
	int32 DestroyIndex = -1;
	int32 CurrentDestroyIndex;

private:
	float bUseInterp = false;
	float InterpStartInterval = 0.1f;
	float Duration = 1.0f;
	float InterpExponent = 1.0f;
	float InterpAlpha = 0.0f;
};
