// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GNRCharacterBase.h"
#include "GameplayTagContainer.h"
#include "GNRPlayerCharacter.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class AGNRWeaponBase;

/**
 * 
 */
UCLASS()
class GNR_API AGNRPlayerCharacter : public AGNRCharacterBase
{
	GENERATED_BODY()
public:
	AGNRPlayerCharacter();

	void EquipWeapon(AGNRWeaponBase* NewWeapon);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void UnequipWeapon();

	AGNRWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bHasWeapon = false;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetHasWeapon(bool InputbHasWeapon) { bHasWeapon = InputbHasWeapon; }

protected:
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
