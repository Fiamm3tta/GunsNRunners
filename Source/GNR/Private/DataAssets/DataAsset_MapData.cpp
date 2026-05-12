// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/DataAsset_MapData.h"

FMapData UDataAsset_MapData::GetMapDataByStageId(FName InStageId) const
{
	for (const FMapData& MapData : MapDatas)
	{
		if (MapData.StageId == InStageId)
		{
			return MapData;
		}
	}

	return FMapData();
}
