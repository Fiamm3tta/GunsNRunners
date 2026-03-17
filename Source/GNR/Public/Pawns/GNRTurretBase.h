// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GNRTurretBase.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
class UCapsuleComponent;
class AGNRProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurretDiedSignature, AGNRTurretBase*, DeadTurret);

UCLASS()
class GNR_API AGNRTurretBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGNRTurretBase();

	UFUNCTION(BlueprintCallable, Category = "Turret|Combat")
	virtual void ApplyDamage(float DamageAmount);

	UFUNCTION(BlueprintPure, Category = "Turret|Combat")
	bool IsDead() const { return bIsDead; }

	UPROPERTY(BlueprintAssignable, Category = "Turret|Event")
	FOnTurretDiedSignature OnTurretDied;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Detection
	UFUNCTION()
	void HandleDetectionBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void HandleDetectionEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void SetCurrentTarget(APawn* NewTarget);
	void ClearCurrentTarget(APawn* TargetToClear);

protected:
	// Aim & Fire
	void UpdateAim(float DeltaTime);
	FVector GetTargetAimLocation() const;
	FRotator GetDesiredAimRotation() const;
	
	bool HasValidTarget() const;
	bool HasLineOfSightToTarget() const;
	bool IsAimAligned() const;
	bool CanFire() const;

	void StartFireLoop();
	void StopFireLoop();

	UFUNCTION()
	void TryFire();

	virtual void FireProjectile();

protected:
	// Death
	virtual void Die();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Component")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Component")
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Component")
	TObjectPtr<UStaticMeshComponent> HeadMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Component")
	TObjectPtr<USceneComponent> MuzzlePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Component")
	TObjectPtr<USphereComponent> DetectionSphere;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Target")
	float DetectionRadius = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Aim")
	float TurnSpeedDegPerSec = 180.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Aim")
	float MaxYawErrorToFire = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Fire")
	float FireInterval = 1.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Fire")
	TSubclassOf<AGNRProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret|Combat")
	float MaxHealth = 1.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Combat")
	float CurrentHealth = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret|Combat")
	bool bIsDead = false;

	UPROPERTY()
	TObjectPtr<APawn> CurrentTarget;

	FTimerHandle FireTimerHandle;

};
