// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/GNRStructTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "GNRWeaponBase.generated.h"

class AGNRPlayerCharacter;
class UWeaponPickUpComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UGameplayEffect;

UCLASS()
class GNR_API AGNRWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGNRWeaponBase();

    virtual void OnEquipped(AGNRPlayerCharacter* NewOwnerCharacter);
    virtual void OnUnequipped();

    bool IsEquipped() const { return bEquipped; }
    AGNRPlayerCharacter* GetOwningPlayerCharacter() const { return OwningPlayerCharacter; }

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void Fire();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SkillStart();

    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void SkillEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UFUNCTION()
    void HandlePickedUp(AGNRPlayerCharacter* PlayerCharacter);

    void AttachToCharacterMesh(AGNRPlayerCharacter* PlayerCharacter);
    void SetPickupEnabled(bool bEnabled);
    void SetWeaponCollisionEnabled(bool bEnabled);

    virtual void FireInternal();
    virtual void SkillStartInternal();
    virtual void SkillEndInternal();

    FVector GetAimPoint(float TraceDistance) const;
    FVector GetDirectionToAimPoint() const;
    

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<USkeletalMeshComponent> WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<USceneComponent> MuzzlePoint;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    TObjectPtr<UWeaponPickUpComponent> PickupComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    FName AttachSocketName = TEXT("WeaponSocket");

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
    bool bEquipped = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    float AimTraceDistance = 10000.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
    bool bUseSkillCooldown = false;

    
    UPROPERTY(Transient)
    TObjectPtr<AGNRPlayerCharacter> OwningPlayerCharacter;
};
