// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

/**
 * 
 */
UCLASS()
class PS_API USkillBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnyWhere, Category = "Skill")
	float skillRange = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	float attackRange = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	int32 apUsage = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	int32 accuracy = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	int32 critical = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	int32 evasion = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	int32 damage = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	float magnification = 1.0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	bool bIsMag = false;

	UPROPERTY(EditAnyWhere, Category = "Script")
	FText skillName = FText::FromString(" ");

	UPROPERTY(EditAnyWhere, Category = "Script")
	FText skillScript = FText::FromString(" ");

	UPROPERTY(EditAnyWhere, Category = "Combat")
	int32 calculatedDamage = 0;

	UPROPERTY(EditAnyWhere, Category = "Combat")
	int32 calculatedAccuracy = 0;

	UPROPERTY(EditAnyWhere, Category = "Combat")
	int32 calculatedCritical = 0;

	UPROPERTY(EditAnyWhere, Category = "Combat")
	int32 calculatedEvasion = 0;
};
