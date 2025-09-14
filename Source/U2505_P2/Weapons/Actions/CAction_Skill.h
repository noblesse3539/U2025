#pragma once

#include "CoreMinimal.h"
#include "Weapons/CActionBase.h"
#include "CAction_Skill.generated.h"

UCLASS(Blueprintable)
class U2505_P2_API UCAction_Skill : public UCActionBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDebug;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Moving")
	float Distance = 800.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Moving")
	float Speed = 100.0f;

public:
	void BeginPlay(class ACharacter* InOwnerCharacter, class ACAttachment* InAttachment, class UCEquipment* InEquipment, const EActionType InActionType) override;
	void Tick(float InDeltaTime) override;
	void Pressed() override;
	void Released() override;

public:
	void Begin_Action_Implementation() override;
	void End_Action_Implementation() override;

private:
	bool	bMoving;
	FVector Start, End;

private:
	struct FPawnCollisionName
	{
		FName			  Name;
		class ACharacter* Character;
	};

private:
	TArray<FPawnCollisionName> Overlapped;

private:
	class UCapsuleComponent* Collision;
};
