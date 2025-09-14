#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CSquadStructures.h"

#include "CSquad.generated.h"

UCLASS()
class U2505_P2_API ACSquad : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Squad")
	class UCSquadAsset* SquadAsset = nullptr;

	UPROPERTY(EditAnywhere, Category = "Squad")
	ESquadKey SquadKey = ESquadKey::None;

	UPROPERTY(EditAnywhere, Category = "Squad")
	int32 TeamId = -1;

	UPROPERTY(EditAnywhere, Category = "Squad")
	FLinearColor Color = FLinearColor::Black;

	UPROPERTY(EditAnywhere, Category = "Squad")
	bool bPaintRandomColor = true;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDebug;

protected:
	virtual void BeginPlay() override;

public:
	void OnOrder(class ACharacter* InTarget);
	void OrderMembers(class ACharacter* InTarget);
	void OrderMembers(ACSquad* InTargetSquad);

public:
	UPROPERTY()
	TArray<class IISquadable*> Members;

	float SpawnDist = 200.0f; // 스폰 시 캐릭터 간 간격
};
