#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCauser, UShapeComponent*, InAttackCollision, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);

class UCEquipment;

UCLASS()
class U2505_P2_API ACAttachment : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USceneComponent* Root;

private:
	UPROPERTY(VisibleAnywhere)
	class UCWeaponTraceComponent* WeaponTrace;

public:
	ACAttachment();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Attachment")
	void AttachTo(FName InSocketName);

protected:
	UFUNCTION(BlueprintCallable, Category = "Attachment")
	void AttachToCollision(FName InCollisionName);

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnEquipmentBeginEquip();
	void OnEquipmentBeginEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnEquipmentUnequip();
	void OnEquipmentUnequip_Implementation() {}

public:
	void SetEquipment(UCEquipment* InEquipment);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Owner")
	class ACharacter* OwnerCharacter;

private:
	TWeakObjectPtr<UCEquipment> Equipment;

public:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	void OnCollision(FName InName);
	void OffCollision();

private:
	UPROPERTY()
	TArray<class UShapeComponent*> Collisions;

public:
	FAttachmentBeginCollision OnAttachmentBeginCollision;
	FAttachmentEndCollision	  OnAttachmentEndCollision;

	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap	OnAttachmentEndOverlap;
};
