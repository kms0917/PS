// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Buff/BuffBase.h"

void UBuffBase::SetBuff(float MoveSpeed,
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
	int32 DamageReinforcement)
{
	this->hp = Hp;
	this->def = Def;
	this->res = Res;
	this->str = Str;
	this->mag = Mag;
	this->skill = Skill;
	this->speed = Speed;
	this->ap = Ap;
	this->critical = Critical;
	this->accuracy = Accuracy;
	this->evade = Evade;
	this->damageReduction_Percent = DamageReduction_Percent;
	this->damageReduction = DamageReduction;
	this->damageReinforcement_Percent = DamageReinforcement_Percent;
	this->damageReinforcement = DamageReinforcement;
	this->moveSpeed = MoveSpeed;
}
