// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Skills/FireBall.h"
#include "Objects/Buff/BuffBase.h"
#include "Actors/ProjectileBase.h"

UFireBall::UFireBall()
{
	skillRange = 1200.0f;
	attackRange = 300.0f;
	apUsage = 1;
	accuracy = 40;
	critical = 0;
	damage = 20;
	magnification = 1.0f;
	bIsMag = true;
	bIsHeal = false;
	bIsBuff = false;
	bIsTargeting = false;
	multiTargetingNum = 1;
	skillName = FText::FromString(TEXT("화염구"));
	skillScript = FText::FromString(TEXT("화염구를 던져 데미지를 준다"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageAsset(TEXT("/Game/Animation/Montages/Standing1HMagicAttack01_UE_Anim_Montage"));
	if (MontageAsset.Succeeded())
	{
		skillMontage = MontageAsset.Object;
	}
	// static ConstructorHelpers::FObjectFinder<UNiagaraSystem> selfEffectParticleFinder(TEXT("/Game/Niagara/Bottom11-01"));
	// if (selfEffectParticleFinder.Succeeded())
	// {
	// 	selfEffectParticle = selfEffectParticleFinder.Object;
	// }
	// static ConstructorHelpers::FObjectFinder<UNiagaraSystem> skillEffectParticleFinder(TEXT("/Game/Niagara"));
	// if (skillEffectParticleFinder.Succeeded())
	// {
	// 	skillEffectParticle = skillEffectParticleFinder.Object;
	// }
	static ConstructorHelpers::FClassFinder<AProjectileBase> ProjectileClassFinder(TEXT("/Game/Actor/Projectiles/BP_FireBallProjectile")); // BP 경로 예시
	if (ProjectileClassFinder.Succeeded())
	{
		projectile = ProjectileClassFinder.Class;
	}
	
	
	PlayRate = 1.0f;
	buff =nullptr;
}
