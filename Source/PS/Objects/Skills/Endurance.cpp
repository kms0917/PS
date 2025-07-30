// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Skills/Endurance.h"
#include "Objects/Buff/BuffBase.h"

UEndurance::UEndurance()
{
	skillRange = 350.0f;
	attackRange = 20.0f;
	apUsage = 1;
	accuracy = 40;
	critical = 0;
	damage = 0;
	magnification = 0.0f;
	bIsMag = false;
	bIsHeal = false;
	bIsBuff = true;
	bIsTargeting = true;
	multiTargetingNum = 1;
	skillName = FText::FromString(TEXT("인내심"));
	skillScript = FText::FromString(TEXT("3턴간 받는 피해 감소"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAsset(TEXT("/Game/Animation/Montages/StandingTauntBattlecry_UE_Anim_Montage"));
	if (MontageAsset.Succeeded())
	{
		skillMontage = MontageAsset.Object;
	}

	PlayRate = 1.0f;
	buff = NewObject<UBuffBase>(this, UBuffBase::StaticClass(), TEXT("Endurance"));
	buff->SetBuff(0,7,5,5,0, 0,0,0,0,0,0,0,0,5,0,0,3);

}
