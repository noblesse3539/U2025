#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMouseComponent.generated.h"

class ACPlayer;
class UCMovementComponent;
class UCTargetComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class U2505_P2_API UCMouseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCMouseComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ShowCursor();
	void HideCursor();

public:
	void OnLeftClick();
	void OnRightClick();

private:
	void SetMouseOver(AActor* InActor);

	void OnHighlight(AActor* InActor);
	void OffHighlight(AActor* InActor);

private:
	TWeakObjectPtr<AActor> MouseOverActor = nullptr;

private:
	bool bShowCursor = false;

private:
	TWeakObjectPtr<ACPlayer>			Owner;
	TWeakObjectPtr<UCMovementComponent> Movement;
	TWeakObjectPtr<UCTargetComponent>	TargetComponent;
};
