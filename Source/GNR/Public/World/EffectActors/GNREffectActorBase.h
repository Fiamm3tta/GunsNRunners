// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GNREffectActorBase.generated.h"

class UBoxComponent;
class UGameplayEffect;
struct FActiveGameplayEffectHandle;
class UAbilitySystemComponent;

UCLASS()
class GNR_API AGNREffectActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGNREffectActorBase();

    void BeginPlay();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EffectActor")
	UBoxComponent* EffectCollisionBox;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EffectActor")
    TSubclassOf<UGameplayEffect> EffectClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EffectActor")
    float EffectLevel = 1.f;

    UFUNCTION()
    void OnEffectBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UFUNCTION()
    void OnEffectEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );

    virtual void HandleEffectBeginOverlap(AActor* TargetActor);
    virtual void HandleEffectEndOverlap(AActor* TargetActor);

    UAbilitySystemComponent* GetTargetASC(AActor* TargetActor) const;

    FActiveGameplayEffectHandle ApplyEffectToTarget(AActor* TargetActor); 
    void RemoveEffectFromTarget(AActor* TargetActor, FActiveGameplayEffectHandle EffectHandle);

    UFUNCTION(BlueprintImplementableEvent)
    void BP_OnEffectApplied(AActor* TargetActor);

    UFUNCTION(BlueprintImplementableEvent)
    void BP_OnEffectRemoved(AActor* TargetActor);
};
