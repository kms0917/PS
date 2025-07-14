// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Skills/Heal.h"

#include "Objects/Buff/BuffBase.h"

UHeal::UHeal()
{
	skillRange = 350.0f;
	attackRange = 20.0f;
	apUsage = 1;
	accuracy = 40;
	critical = 0;
	damage = 0;
	magnification = 1.0f;
	bIsMag = false;
	bIsHeal = true;
	bIsTargeting = true;
	multiTargetingNum = 1;
	skillName = FText::FromString(TEXT("회복"));
	skillScript = FText::FromString(TEXT("마력에 비례해 대상을 회복시킨다."));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAsset(TEXT("/Game/Animation/Montages/Standing1HMagicAttack03_UE_Anim_Montage"));
	if (MontageAsset.Succeeded())
	{
		skillMontage = MontageAsset.Object;
	}

	PlayRate = 1.0f;
	buff = NewObject<UBuffBase>(this, UBuffBase::StaticClass(), TEXT("Heal"));
	buff->SetBuff(0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,3);
}
