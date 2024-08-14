// Fill out your copyright notice in the Description page of Project Settings.


#include "McBotData.h"

FPrimaryAssetId UMcBotData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("BotData", GetFName());
}
