// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "GoblinWarrior.generated.h"

/**
 * 
 */
UCLASS()
class PS_API AGoblinWarrior : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	AGoblinWarrior();

	virtual void SetDefaultEquipments() override;
	virtual void SetDefaultSkills() override;
};
