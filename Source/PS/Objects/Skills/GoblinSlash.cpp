// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Skills/GoblinSlash.h"

UGoblinSlash::UGoblinSlash()
{
	skillRange = 200.0f;
	attackRange = 50.0f;
	apUsage = 1;
	accuracy = 40;
	critical = 0;
	damage = 0;
	magnification = 1.0f;
	bIsMag = false;
	bIsHeal = false;
	multiTargetingNum = 1;
	skillName = FText::FromString(TEXT("기본 공격"));
	skillScript = FText::FromString(TEXT("힘에 비례한 물리 공격"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAsset(TEXT("/Game/Animation/Montages/GreatSwordSlash_UE_Anim_Montage"));
	if (MontageAsset.Succeeded())
	{
		skillMontage = MontageAsset.Object;
	}

	PlayRate = 1.0f;
	buff = nullptr;
}
