// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/GoblinWarrior.h"
#include "ActorComponent/EquipmentComponent.h"
#include "Objects/Weapons/GoblinSword.h"
#include "ActorComponent/SkillComponent.h"
#include "Objects/Skills/GoblinSlash.h"

#include "GenericTeamAgentInterface.h"

AGoblinWarrior::AGoblinWarrior()
{
	hp = 25;
	str = 10;
	mag = 2;
	skill = 5;
	speed = 7;
	def = 6;
	res = 4;
	moveSpeed = 8;
	hpGrowth = 60;
	strGrowth = 60;
	magGrowth = 25;
	defGrowth = 45;
	resGrowth = 35;
	skillGrowth = 40;
	speedGrowth = 50;
	ap = 1;
	SetDefaultEquipments();
	SetDefaultSkills();

	SetStats();
	SetSkillInfo();
}

void AGoblinWarrior::BeginPlay()
{
	Super::BeginPlay();

	SetGenericTeamId(FGenericTeamId(1));
}

void AGoblinWarrior::SetDefaultEquipments()
{
	UGoblinSword* defaultWeapon = NewObject<UGoblinSword>(this, UGoblinSword::StaticClass(), TEXT("Goblin Sword"));
	equipmentComponent->EquipWeapon(defaultWeapon);
}

void AGoblinWarrior::SetDefaultSkills()
{
	UGoblinSlash* defaultSkill = NewObject<UGoblinSlash>(this, UGoblinSlash::StaticClass(), TEXT("Goblin Slash"));
	skillComponent->AddSkill(defaultSkill);
}
