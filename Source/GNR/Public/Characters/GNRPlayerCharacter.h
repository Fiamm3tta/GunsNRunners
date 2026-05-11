// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GNRCharacterBase.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "GNRPlayerCharacter.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class AGNRWeaponBase;
class UPlayerUIComponent;

/**
 * 
 */
UCLASS()
class GNR_API AGNRPlayerCharacter : public AGNRCharacterBase
{
	GENERATED_BODY()
public:
	AGNRPlayerCharacter();

#pragma region Weapon Equip
	void EquipWeapon(AGNRWeaponBase* NewWeapon);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UnequipWeapon();

	UFUNCTION(BlueprintCallable)
	AGNRWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bHasWeapon = false;

#pragma endregion
	/*
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetHasWeapon(bool InputbHasWeapon) { bHasWeapon = InputbHasWeapon; }
	*/

#pragma region Zoom
	bool bIsZooming = false;

	float DefaultFOV = 90.0f;
	float ZoomFOV = 60.0f;
	float ZoomInterpSpeed = 12.0f;

	float DefaultLookSensitivity = 1.0f;
	float ZoomLookSensitivity = 0.5f;

	void StartZoom();
	void StopZoom();
#pragma endregion

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UPlayerUIComponent* GetPlayerUIComponent() const override;
	//~ End IPawnUIInterface Interface

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	void OnCurrentSpeedChanged(const FOnAttributeChangeData& Data);

	void ApplyCurrentSpeedToMovement(float NewSpeed);

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
#pragma region Components
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UPlayerUIComponent* PlayerUIComponent;
#pragma endregion

#pragma region Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
#pragma endregion

public:
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<AGNRWeaponBase> CurrentWeapon;
};
