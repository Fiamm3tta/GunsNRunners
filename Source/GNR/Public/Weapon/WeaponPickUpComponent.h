// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "WeaponPickUpComponent.generated.h"

class AGNRPlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponPickedUp, AGNRPlayerCharacter*, PlayerCharacter);

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GNR_API UWeaponPickUpComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UWeaponPickUpComponent();

	UPROPERTY(BlueprintAssignable, Category = "Pickup")
	FOnWeaponPickedUp OnWeaponPickedUp;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
	
};
