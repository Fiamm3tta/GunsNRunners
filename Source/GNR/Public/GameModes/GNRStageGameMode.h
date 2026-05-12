// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/GNRGameModeBase.h"
#include "GNRStageGameMode.generated.h"

class UGNRGameInstance;
class AGNRGameStateBase;

/**
 * 
 */
UCLASS()
class GNR_API AGNRStageGameMode : public AGNRGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void HandleStageClear();
	
protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Transient)
	TObjectPtr<UGNRGameInstance> CurrentGameInstance;

	UPROPERTY(Transient)
	TObjectPtr<AGNRGameStateBase> CurrentGameState;

	FName CurrentStageId = TEXT("Stage1");

	bool bHard = false;
};
