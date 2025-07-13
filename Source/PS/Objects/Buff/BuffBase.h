// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BuffBase.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UBuffBase : public UObject
{
	GENERATED_BODY()
public:
	void SetBuff(float MoveSpeed,
	int32 Hp,
	int32 Def,
	int32 Res,
	int32 Str,
	int32 Mag,
	int32 Skill,
	int32 Speed,
	int32 Ap,
	int32 Critical,
	int32 Accuracy,
	int32 Evade,
	float DamageReduction_Percent,
	int32 DamageReduction,
	float DamageReinforcement_Percent,
	int32 DamageReinforcement);

	float moveSpeed;
	int32 hp;
	int32 def;
	int32 res;
	int32 str;
	int32 mag;
	int32 skill;
	int32 speed;
	int32 ap;
	int32 critical;
	int32 accuracy;
	int32 evade;
	float damageReduction_Percent;
	int32 damageReduction;
	float damageReinforcement_Percent;
	int32 damageReinforcement;
};
