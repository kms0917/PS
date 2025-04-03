// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/EquipmentComponent.h"
#include "Objects/BootsBase.h"
#include "Objects/ChestBase.h"
#include "Objects/GloveBase.h"
#include "Objects/HelmetBase.h"
#include "Objects/LegBase.h"
#include "Objects/WeaponBase.h"
#include "Character/CharacterBase.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* ownerActor = GetOwner();

	if (ownerActor)
	{
		ACharacterBase* MyCharacter = Cast<ACharacterBase>(ownerActor);
		if (MyCharacter)
		{
			ownerCharacter = MyCharacter;
		}
	}
	CalcEquipmentStats();
}

void UEquipmentComponent::EquipWeapon(UWeaponBase* EquipedWeapon)
{
	if (weapon)
	{
		UnEquipWeapon();
	}
	weapon = EquipedWeapon; 
	equipmentHp += weapon->hp;
	equipmentStr += weapon->str;
	equipmentMag += weapon->mag;
	equipmentDef += weapon->def;
	equipmentRes += weapon->res;
	equipmentSkill += weapon->skill;
	equipmentSpeed += weapon->speed;
	equipmentMoveSpeed += weapon->moveSpeed;
	equipmentDamage += weapon->damage;
	equipmentAccuracy += weapon->accuracy;
	equipmentEvasion += weapon->evasion;
	equipmentCritical += weapon->critical;
	equipmentAp += weapon->ap;
	if (ownerCharacter)
	{
		weapon->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::EquipHelmet(UHelmetBase* EquipedHelmet)
{
	if (helmet)
	{
		UnEquipHelmet();
	}
	helmet = EquipedHelmet;
	equipmentHp += helmet->hp;
	equipmentStr += helmet->str;
	equipmentMag += helmet->mag;
	equipmentDef += helmet->def;
	equipmentRes += helmet->res;
	equipmentSkill += helmet->skill;
	equipmentSpeed += helmet->speed;
	equipmentMoveSpeed += helmet->moveSpeed;
	equipmentDamage += helmet->damage;
	equipmentAccuracy += helmet->accuracy;
	equipmentEvasion += helmet->evasion;
	equipmentCritical += helmet->critical;
	equipmentAp += helmet->ap;
	if (ownerCharacter)
	{
		helmet->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::EquipChest(UChestBase* EquipedChest)
{
	if (chest)
	{
		UnEquipChest();
	}
	chest = EquipedChest;
	equipmentHp += chest->hp;
	equipmentStr += chest->str;
	equipmentMag += chest->mag;
	equipmentDef += chest->def;
	equipmentRes += chest->res;
	equipmentSkill += chest->skill;
	equipmentSpeed += chest->speed;
	equipmentMoveSpeed += chest->moveSpeed;
	equipmentDamage += chest->damage;
	equipmentAccuracy += chest->accuracy;
	equipmentEvasion += chest->evasion;
	equipmentCritical += chest->critical;
	equipmentAp += chest->ap;
	if (ownerCharacter)
	{
		chest->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::EquipGlove(UGloveBase* EquipedGlove)
{
	if (glove)
	{
		UnEquipGlove();
	}
	glove = EquipedGlove;
	equipmentHp += glove->hp;
	equipmentStr += glove->str;
	equipmentMag += glove->mag;
	equipmentDef += glove->def;
	equipmentRes += glove->res;
	equipmentSkill += glove->skill;
	equipmentSpeed += glove->speed;
	equipmentMoveSpeed += glove->moveSpeed;
	equipmentDamage += glove->damage;
	equipmentAccuracy += glove->accuracy;
	equipmentEvasion += glove->evasion;
	equipmentCritical += glove->critical;
	equipmentAp += glove->ap;
	if (ownerCharacter)
	{
		glove->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::EquipLeg(ULegBase* EquipedLeg)
{
	if (leg)
	{
		UnEquipLeg();
	}
	leg = EquipedLeg;
	equipmentHp += leg->hp;
	equipmentStr += leg->str;
	equipmentMag += leg->mag;
	equipmentDef += leg->def;
	equipmentRes += leg->res;
	equipmentSkill += leg->skill;
	equipmentSpeed += leg->speed;
	equipmentMoveSpeed += leg->moveSpeed;
	equipmentDamage += leg->damage;
	equipmentAccuracy += leg->accuracy;
	equipmentEvasion += leg->evasion;
	equipmentCritical += leg->critical;
	equipmentAp += leg->ap;
	if (ownerCharacter)
	{
		leg->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::EquipBoots(UBootsBase* EquipedBoots)
{
	if (boots)
	{
		UnEquipBoots();
	}
	boots = EquipedBoots;
	equipmentHp += boots->hp;
	equipmentStr += boots->str;
	equipmentMag += boots->mag;
	equipmentDef += boots->def;
	equipmentRes += boots->res;
	equipmentSkill += boots->skill;
	equipmentSpeed += boots->speed;
	equipmentMoveSpeed += boots->moveSpeed;
	equipmentDamage += boots->damage;
	equipmentAccuracy += boots->accuracy;
	equipmentEvasion += boots->evasion;
	equipmentCritical += boots->critical;
	equipmentAp += boots->ap;
	if (ownerCharacter)
	{
		boots->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::UnEquipWeapon()
{
	equipmentHp -= weapon->hp;
	equipmentStr -= weapon->str;
	equipmentMag -= weapon->mag;
	equipmentDef -= weapon->def;
	equipmentRes -= weapon->res;
	equipmentSkill -= weapon->skill;
	equipmentSpeed -= weapon->speed;
	equipmentMoveSpeed -= weapon->moveSpeed;
	equipmentDamage -= weapon->damage;
	equipmentAccuracy -= weapon->accuracy;
	equipmentEvasion -= weapon->evasion;
	equipmentCritical -= weapon->critical;
	equipmentAp -= weapon->ap;
	weapon->ownerCharacter = nullptr;
	weapon = nullptr;
}

void UEquipmentComponent::UnEquipHelmet()
{
	equipmentHp -= helmet->hp;
	equipmentStr -= helmet->str;
	equipmentMag -= helmet->mag;
	equipmentDef -= helmet->def;
	equipmentRes -= helmet->res;
	equipmentSkill -= helmet->skill;
	equipmentSpeed -= helmet->speed;
	equipmentMoveSpeed -= helmet->moveSpeed;
	equipmentDamage -= helmet->damage;
	equipmentAccuracy -= helmet->accuracy;
	equipmentEvasion -= helmet->evasion;
	equipmentCritical -= helmet->critical;
	equipmentAp -= helmet->ap;
	helmet->ownerCharacter = nullptr;
	helmet = nullptr;
}

void UEquipmentComponent::UnEquipChest()
{
	equipmentHp -= chest->hp;
	equipmentStr -= chest->str;
	equipmentMag -= chest->mag;
	equipmentDef -= chest->def;
	equipmentRes -= chest->res;
	equipmentSkill -= chest->skill;
	equipmentSpeed -= chest->speed;
	equipmentMoveSpeed -= chest->moveSpeed;
	equipmentDamage -= chest->damage;
	equipmentAccuracy -= chest->accuracy;
	equipmentEvasion -= chest->evasion;
	equipmentCritical -= chest->critical;
	equipmentAp -= chest->ap;
	chest->ownerCharacter = nullptr;
	chest = nullptr;
}

void UEquipmentComponent::UnEquipGlove()
{
	equipmentHp -= glove->hp;
	equipmentStr -= glove->str;
	equipmentMag -= glove->mag;
	equipmentDef -= glove->def;
	equipmentRes -= glove->res;
	equipmentSkill -= glove->skill;
	equipmentSpeed -= glove->speed;
	equipmentMoveSpeed -= glove->moveSpeed;
	equipmentDamage -= glove->damage;
	equipmentAccuracy -= glove->accuracy;
	equipmentEvasion -= glove->evasion;
	equipmentCritical -= glove->critical;
	equipmentAp -= glove->ap;
	glove->ownerCharacter = nullptr;
	glove = nullptr;
}

void UEquipmentComponent::UnEquipLeg()
{
	equipmentHp -= leg->hp;
	equipmentStr -= leg->str;
	equipmentMag -= leg->mag;
	equipmentDef -= leg->def;
	equipmentRes -= leg->res;
	equipmentSkill -= leg->skill;
	equipmentSpeed -= leg->speed;
	equipmentMoveSpeed -= leg->moveSpeed;
	equipmentDamage -= leg->damage;
	equipmentAccuracy -= leg->accuracy;
	equipmentEvasion -= leg->evasion;
	equipmentCritical -= leg->critical;
	equipmentAp -= leg->ap;
	leg->ownerCharacter = nullptr;
	leg = nullptr;
}

void UEquipmentComponent::UnEquipBoots()
{
	equipmentHp -= boots->hp;
	equipmentStr -= boots->str;
	equipmentMag -= boots->mag;
	equipmentDef -= boots->def;
	equipmentRes -= boots->res;
	equipmentSkill -= boots->skill;
	equipmentSpeed -= boots->speed;
	equipmentMoveSpeed -= boots->moveSpeed;
	equipmentDamage -= boots->damage;
	equipmentAccuracy -= boots->accuracy;
	equipmentEvasion -= boots->evasion;
	equipmentCritical -= boots->critical;
	equipmentAp -= boots->ap;
	boots->ownerCharacter = nullptr;
	boots = nullptr;
}

void UEquipmentComponent::CalcEquipmentStats()
{
	int32 Hp = 0;
	int32 Str = 0;
	int32 Mag = 0;
	int32 Def = 0;
	int32 Res = 0;
	int32 Skill = 0;
	int32 Speed = 0;
	float MoveSpeed = 0;
	int32 Damage = 0;
	int32 Accuracy = 0;
	int32 Evasion = 0;
	int32 Critical = 0;
	int32 Ap = 0;
	if (weapon)
	{
		Hp += weapon->hp;
		Str += weapon->str;
		Mag += weapon->mag;
		Def += weapon->def;
		Res += weapon->res;
		Skill += weapon->skill;
		Speed += weapon->speed;
		MoveSpeed += weapon->moveSpeed;
		Damage += weapon->damage;
		Accuracy += weapon->accuracy;
		Evasion += weapon->evasion;
		Critical += weapon->critical;
		Ap += weapon->ap;
	}
	if (helmet)
	{
		Hp += helmet->hp;
		Str += helmet->str;
		Mag += helmet->mag;
		Def += helmet->def;
		Res += helmet->res;
		Skill += helmet->skill;
		Speed += helmet->speed;
		MoveSpeed += helmet->moveSpeed;
		Damage += helmet->damage;
		Accuracy += helmet->accuracy;
		Evasion += helmet->evasion;
		Critical += helmet->critical;
		Ap += helmet->ap;
	}
	if (chest)
	{
		Hp += chest->hp;
		Str += chest->str;
		Mag += chest->mag;
		Def += chest->def;
		Res += chest->res;
		Skill += chest->skill;
		Speed += chest->speed;
		MoveSpeed += chest->moveSpeed;
		Damage += chest->damage;
		Accuracy += chest->accuracy;
		Evasion += chest->evasion;
		Critical += chest->critical;
		Ap += chest->ap;
	}
	if (glove)
	{
		Hp += glove->hp;
		Str += glove->str;
		Mag += glove->mag;
		Def += glove->def;
		Res += glove->res;
		Skill += glove->skill;
		Speed += glove->speed;
		MoveSpeed += glove->moveSpeed;
		Damage += glove->damage;
		Accuracy += glove->accuracy;
		Evasion += glove->evasion;
		Critical += glove->critical;
		Ap += glove->ap;
	}
	if (leg)
	{
		Hp += leg->hp;
		Str += leg->str;
		Mag += leg->mag;
		Def += leg->def;
		Res += leg->res;
		Skill += leg->skill;
		Speed += leg->speed;
		MoveSpeed += leg->moveSpeed;
		Damage += leg->damage;
		Accuracy += leg->accuracy;
		Evasion += leg->evasion;
		Critical += leg->critical;
		Ap += leg->ap;
	}
	if (boots)
	{
		Hp += boots->hp;
		Str += boots->str;
		Mag += boots->mag;
		Def += boots->def;
		Res += boots->res;
		Skill += boots->skill;
		Speed += boots->speed;
		MoveSpeed += boots->moveSpeed;
		Damage += boots->damage;
		Accuracy += boots->accuracy;
		Evasion += boots->evasion;
		Critical += boots->critical;
		Ap += boots->ap;
	}
	equipmentHp = Hp;
	equipmentStr = Str;
	equipmentMag = Mag;
	equipmentDef = Def;
	equipmentRes = Res;
	equipmentSkill = Skill;
	equipmentSpeed = Speed;
	equipmentMoveSpeed = MoveSpeed;
	equipmentDamage = Damage;
	equipmentAccuracy = Accuracy;
	equipmentEvasion = Evasion;
	equipmentCritical = Critical;
	equipmentAp = Ap;
}
