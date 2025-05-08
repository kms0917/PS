// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Skills/BasicAttack.h"

UBasicAttack::UBasicAttack()
{
	skillRange = 100.0f;
	attackRange = 50.0f;
	apUsage = 1;
	accuracy = 40;
	critical = 0;
	evasion = 0;
	damage = 0;
	magnification = 1.0f;
	bIsMag = false;
	skillName = FText::FromString(TEXT("기본 공격"));
	skillScript = FText::FromString(TEXT("힘에 비례한 물리 공격"));
}