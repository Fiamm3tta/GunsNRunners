// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/GNRMainPlayerController.h"

AGNRMainPlayerController::AGNRMainPlayerController()
{
	PlayerTeamID = FGenericTeamId(0);
}

FGenericTeamId AGNRMainPlayerController::GetGenericTeamId() const
{
	return PlayerTeamID;
}