// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Notifies/CAnimNotify.h"
#include "CAnimNotify_Gravity.generated.h"

/**
 *
 */
UCLASS()
class U2505_P2_API UCAnimNotify_Gravity : public UCAnimNotify
{
	GENERATED_BODY()

public:
	UCAnimNotify_Gravity();

public:
	FString GetNotifyName_Implementation() const override;
	void	Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
