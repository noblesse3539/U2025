#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CStateStructures.h"
#include "CUserWidget_State.generated.h"

UCLASS()
class U2505_P2_API UCUserWidget_State : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateControllerName(const FString& InName);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateState(EStateType InType);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateAIState(EAIStateType InType);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateWeaponState(EWeaponType InType);
};
