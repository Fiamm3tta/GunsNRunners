// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "GNRMainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GNR_API AGNRMainPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	AGNRMainPlayerController();

	//~ Begin IGenericTeamAgentInterface Interface.
	virtual FGenericTeamId GetGenericTeamId() const;
	//~ End IGenericTeamAgentInterface Interface

private:
	FGenericTeamId PlayerTeamID;
};
