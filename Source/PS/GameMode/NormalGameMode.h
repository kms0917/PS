// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NormalGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PS_API ANormalGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ANormalGameMode();

	bool bIsBattle = true;
};
