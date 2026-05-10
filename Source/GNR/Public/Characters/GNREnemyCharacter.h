// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GNRCharacterBase.h"
#include "GameplayEffectTypes.h"
#include "GNREnemyCharacter.generated.h"

class UEnemyCombatComponent;
class UEnemyUIComponent;

/**
 * 
 */
UCLASS()
class GNR_API AGNREnemyCharacter : public AGNRCharacterBase
{
	GENERATED_BODY()
	
public:
	AGNREnemyCharacter();

	//~ Begin IPawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent() const override;
	//~ End IPawnUIInterface Interface

protected:
	virtual void BeginPlay() override;

	void OnCurrentSpeedChanged(const FOnAttributeChangeData& Data);

	void ApplyCurrentSpeedToMovement(float NewSpeed);

	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UEnemyUIComponent* EnemyUIComponent;

private:
	void InitEnemyStartUpData();
	
public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
};
