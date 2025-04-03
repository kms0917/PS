// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "Warrior.generated.h"

/**
 * 
 */
UCLASS()
class PS_API AWarrior : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AWarrior();

protected:

public:
	virtual void SetDefaultEquipments() override;
	virtual void SetDefaultSkills() override;
};
