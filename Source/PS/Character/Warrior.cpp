// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Warrior.h"
#include "ActorComponent/EquipmentComponent.h"
#include "Objects/Weapons/IronSword.h"
#include "Objects/Helmets/IronHelmet.h"
#include "Objects/Chests/IronChest.h"
#include "Objects/Gloves/IronGlove.h"
#include "Objects/Legs/IronLeg.h"
#include "Objects/Boots/IronBoots.h"
#include "ActorComponent/SkillComponent.h"
#include "Objects/Skills/BasicAttack.h"

#include "GenericTeamAgentInterface.h"

AWarrior::AWarrior()
{
	hp = 28;
	str = 12;
	mag = 4;
	skill = 7;
	speed = 7;
	def = 8;
	res = 5;
	moveSpeed = 10;
	hpGrowth = 70;
	strGrowth = 65;
	magGrowth = 25;
	defGrowth = 45;
	resGrowth = 35;
	skillGrowth = 55;
	speedGrowth = 55;
	ap = 2;
	SetDefaultEquipments();
	SetDefaultSkills();

	SetStats();
	SetSkillInfo();
	TeamId = FGenericTeamId(0);
}

void AWarrior::SetDefaultEquipments()	//추후 인벤토리 생기면 인벤토리에도 장비 추가해야함
{
	UIronSword* defaultWeapon = NewObject<UIronSword>(this, UIronSword::StaticClass(), TEXT("Iron Sword"));
	equipmentComponent->EquipWeapon(defaultWeapon);
	UIronHelmet* defaultHelmet = NewObject<UIronHelmet>(this, UIronHelmet::StaticClass(), TEXT("Iron Helmet"));
	equipmentComponent->EquipHelmet(defaultHelmet);
	UIronChest* defaultChest = NewObject<UIronChest>(this, UIronChest::StaticClass(), TEXT("Iron Chest"));
	equipmentComponent->EquipChest(defaultChest);
	UIronGlove* defaultGlove = NewObject<UIronGlove>(this, UIronGlove::StaticClass(), TEXT("Iron Glove"));
	equipmentComponent->EquipGlove(defaultGlove);
	UIronLeg* defaultLeg = NewObject<UIronLeg>(this, UIronLeg::StaticClass(), TEXT("Iron Leg"));
	equipmentComponent->EquipLeg(defaultLeg);
	UIronBoots* defaultBoots = NewObject<UIronBoots>(this, UIronBoots::StaticClass(), TEXT("Iron Boots"));
	equipmentComponent->EquipBoots(defaultBoots);
}

void AWarrior::SetDefaultSkills()
{
	UBasicAttack* defaultSkill = NewObject<UBasicAttack>(this, UBasicAttack::StaticClass(), TEXT("Basic Attack"));
	skillComponent->AddSkill(defaultSkill);

}
