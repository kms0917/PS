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
	CalcEquipmentStats();
	ownerCharacter->currentHp += weapon->hp;
	ownerCharacter->hp += weapon->hp;
	ownerCharacter->currentStr += weapon->str;
	ownerCharacter->currentMag += weapon->mag;
	ownerCharacter->currentDef += weapon->def;
	ownerCharacter->currentRes += weapon->res;
	ownerCharacter->currentSkill += weapon->skill;
	ownerCharacter->currentSpeed += weapon->speed;
	ownerCharacter->currentMoveSpeed += weapon->moveSpeed;
	ownerCharacter->currentAp += weapon->ap;
	ownerCharacter->damageReduction += weapon->damageReduction;
	ownerCharacter->damageReduction_Percent += weapon->damageReduction_percent;
	ownerCharacter->damageReinforcement += weapon->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent += weapon->damageReinforcement_percent;
	if (ownerCharacter)
	{
		weapon->ownerCharacter = ownerCharacter;
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::EquipHelmet(UHelmetBase* EquipedHelmet)
{
	if (helmet)
	{
		UnEquipHelmet();
	}
	helmet = EquipedHelmet;
	CalcEquipmentStats();
	ownerCharacter->currentHp += helmet->hp;
	ownerCharacter->hp += helmet->hp;
	ownerCharacter->currentStr += helmet->str;
	ownerCharacter->currentMag += helmet->mag;
	ownerCharacter->currentDef += helmet->def;
	ownerCharacter->currentRes += helmet->res;
	ownerCharacter->currentSkill += helmet->skill;
	ownerCharacter->currentSpeed += helmet->speed;
	ownerCharacter->currentMoveSpeed += helmet->moveSpeed;
	ownerCharacter->currentAp += helmet->ap;
	ownerCharacter->damageReduction += helmet->damageReduction;
	ownerCharacter->damageReduction_Percent += helmet->damageReduction_percent;
	ownerCharacter->damageReinforcement += helmet->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent += helmet->damageReinforcement_percent;
	if (ownerCharacter)
	{
		helmet->ownerCharacter = ownerCharacter;
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::EquipChest(UChestBase* EquipedChest)
{
	if (chest)
	{
		UnEquipChest();
	}
	chest = EquipedChest;
	CalcEquipmentStats();
	ownerCharacter->currentHp += chest->hp;
	ownerCharacter->hp += chest->hp;
	ownerCharacter->currentStr += chest->str;
	ownerCharacter->currentMag += chest->mag;
	ownerCharacter->currentDef += chest->def;
	ownerCharacter->currentRes += chest->res;
	ownerCharacter->currentSkill += chest->skill;
	ownerCharacter->currentSpeed += chest->speed;
	ownerCharacter->currentMoveSpeed += chest->moveSpeed;
	ownerCharacter->currentAp += chest->ap;
	ownerCharacter->damageReduction += chest->damageReduction;
	ownerCharacter->damageReduction_Percent += chest->damageReduction_percent;
	ownerCharacter->damageReinforcement += chest->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent += chest->damageReinforcement_percent;
	if (ownerCharacter)
	{
		chest->ownerCharacter = ownerCharacter;
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::EquipGlove(UGloveBase* EquipedGlove)
{
	if (glove)
	{
		UnEquipGlove();
	}
	glove = EquipedGlove;
	CalcEquipmentStats();
	ownerCharacter->currentHp += glove->hp;
	ownerCharacter->hp += glove->hp;
	ownerCharacter->currentStr += glove->str;
	ownerCharacter->currentMag += glove->mag;
	ownerCharacter->currentDef += glove->def;
	ownerCharacter->currentRes += glove->res;
	ownerCharacter->currentSkill += glove->skill;
	ownerCharacter->currentSpeed += glove->speed;
	ownerCharacter->currentMoveSpeed += glove->moveSpeed;
	ownerCharacter->currentAp += glove->ap;
	ownerCharacter->damageReduction += glove->damageReduction;
	ownerCharacter->damageReduction_Percent += glove->damageReduction_percent;
	ownerCharacter->damageReinforcement += glove->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent += glove->damageReinforcement_percent;
	if (ownerCharacter)
	{
		glove->ownerCharacter = ownerCharacter;
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::EquipLeg(ULegBase* EquipedLeg)
{
	if (leg)
	{
		UnEquipLeg();
	}
	leg = EquipedLeg;
	CalcEquipmentStats();
	ownerCharacter->currentHp += leg->hp;
	ownerCharacter->hp += leg->hp;
	ownerCharacter->currentStr += leg->str;
	ownerCharacter->currentMag += leg->mag;
	ownerCharacter->currentDef += leg->def;
	ownerCharacter->currentRes += leg->res;
	ownerCharacter->currentSkill += leg->skill;
	ownerCharacter->currentSpeed += leg->speed;
	ownerCharacter->currentMoveSpeed += leg->moveSpeed;
	ownerCharacter->currentAp += leg->ap;
	ownerCharacter->damageReduction += leg->damageReduction;
	ownerCharacter->damageReduction_Percent += leg->damageReduction_percent;
	ownerCharacter->damageReinforcement += leg->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent += leg->damageReinforcement_percent;
	if (ownerCharacter)
	{
		leg->ownerCharacter = ownerCharacter;
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::EquipBoots(UBootsBase* EquipedBoots)
{
	if (boots)
	{
		UnEquipBoots();
	}
	boots = EquipedBoots;
	CalcEquipmentStats();
	ownerCharacter->currentHp += boots->hp;
	ownerCharacter->hp += boots->hp;
	ownerCharacter->currentStr += boots->str;
	ownerCharacter->currentMag += boots->mag;
	ownerCharacter->currentDef += boots->def;
	ownerCharacter->currentRes += boots->res;
	ownerCharacter->currentSkill += boots->skill;
	ownerCharacter->currentSpeed += boots->speed;
	ownerCharacter->currentMoveSpeed += boots->moveSpeed;
	ownerCharacter->currentAp += boots->ap;
	ownerCharacter->damageReduction += boots->damageReduction;
	ownerCharacter->damageReduction_Percent += boots->damageReduction_percent;
	ownerCharacter->damageReinforcement += boots->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent += boots->damageReinforcement_percent;
	if (ownerCharacter)
	{
		boots->ownerCharacter = ownerCharacter;
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::UnEquipWeapon()
{
	if (weapon == nullptr)
	{
		return;
	}
	ownerCharacter->hp -= weapon->hp;
	ownerCharacter->currentStr -= weapon->str;
	ownerCharacter->currentMag -= weapon->mag;
	ownerCharacter->currentDef -= weapon->def;
	ownerCharacter->currentRes -= weapon->res;
	ownerCharacter->currentSkill -= weapon->skill;
	ownerCharacter->currentSpeed -= weapon->speed;
	ownerCharacter->currentMoveSpeed -= weapon->moveSpeed;
	ownerCharacter->currentAp -= weapon->ap;
	ownerCharacter->damageReduction -= weapon->damageReduction;
	ownerCharacter->damageReduction_Percent -= weapon->damageReduction_percent;
	ownerCharacter->damageReinforcement -= weapon->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent -= weapon->damageReinforcement_percent;
	weapon->ownerCharacter = nullptr;
	weapon = nullptr;
	CalcEquipmentStats();
	if (ownerCharacter)
	{
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::UnEquipHelmet()
{
	if (helmet == nullptr)
	{
		return;
	}
	ownerCharacter->hp -= helmet->hp;
	ownerCharacter->currentStr -= helmet->str;
	ownerCharacter->currentMag -= helmet->mag;
	ownerCharacter->currentDef -= helmet->def;
	ownerCharacter->currentRes -= helmet->res;
	ownerCharacter->currentSkill -= helmet->skill;
	ownerCharacter->currentSpeed -= helmet->speed;
	ownerCharacter->currentMoveSpeed -= helmet->moveSpeed;
	ownerCharacter->currentAp -= helmet->ap;
	ownerCharacter->damageReduction -= helmet->damageReduction;
	ownerCharacter->damageReduction_Percent -= helmet->damageReduction_percent;
	ownerCharacter->damageReinforcement -= helmet->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent -= helmet->damageReinforcement_percent;
	helmet->ownerCharacter = nullptr;
	helmet = nullptr;
	CalcEquipmentStats();
	if (ownerCharacter)
	{
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::UnEquipChest()
{
	if (chest == nullptr)
	{
		return;
	}
	ownerCharacter->hp -= chest->hp;
	ownerCharacter->currentStr -= chest->str;
	ownerCharacter->currentMag -= chest->mag;
	ownerCharacter->currentDef -= chest->def;
	ownerCharacter->currentRes -= chest->res;
	ownerCharacter->currentSkill -= chest->skill;
	ownerCharacter->currentSpeed -= chest->speed;
	ownerCharacter->currentMoveSpeed -= chest->moveSpeed;
	ownerCharacter->currentAp -= chest->ap;
	ownerCharacter->damageReduction -= chest->damageReduction;
	ownerCharacter->damageReduction_Percent -= chest->damageReduction_percent;
	ownerCharacter->damageReinforcement -= chest->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent -= chest->damageReinforcement_percent;
	chest->ownerCharacter = nullptr;
	chest = nullptr;
	CalcEquipmentStats();
	if (ownerCharacter)
	{
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::UnEquipGlove()
{
	if (glove == nullptr)
	{
		return;
	}
	ownerCharacter->hp -= glove->hp;
	ownerCharacter->currentStr -= glove->str;
	ownerCharacter->currentMag -= glove->mag;
	ownerCharacter->currentDef -= glove->def;
	ownerCharacter->currentRes -= glove->res;
	ownerCharacter->currentSkill -= glove->skill;
	ownerCharacter->currentSpeed -= glove->speed;
	ownerCharacter->currentMoveSpeed -= glove->moveSpeed;
	ownerCharacter->currentAp -= glove->ap;
	ownerCharacter->damageReduction -= glove->damageReduction;
	ownerCharacter->damageReduction_Percent -= glove->damageReduction_percent;
	ownerCharacter->damageReinforcement -= glove->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent -= glove->damageReinforcement_percent;
	glove->ownerCharacter = nullptr;
	glove = nullptr;
	CalcEquipmentStats();
	if (ownerCharacter)
	{
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::UnEquipLeg()
{
	if (leg == nullptr)
	{
		return;
	}
	ownerCharacter->hp -= leg->hp;
	ownerCharacter->currentStr -= leg->str;
	ownerCharacter->currentMag -= leg->mag;
	ownerCharacter->currentDef -= leg->def;
	ownerCharacter->currentRes -= leg->res;
	ownerCharacter->currentSkill -= leg->skill;
	ownerCharacter->currentSpeed -= leg->speed;
	ownerCharacter->currentMoveSpeed -= leg->moveSpeed;
	ownerCharacter->currentAp -= leg->ap;
	ownerCharacter->damageReduction -= leg->damageReduction;
	ownerCharacter->damageReduction_Percent -= leg->damageReduction_percent;
	ownerCharacter->damageReinforcement -= leg->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent -= leg->damageReinforcement_percent;
	leg->ownerCharacter = nullptr;
	leg = nullptr;
	CalcEquipmentStats();
	if (ownerCharacter)
	{
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

void UEquipmentComponent::UnEquipBoots()
{
	if (boots == nullptr)
	{
		return;
	}
	ownerCharacter->hp -= boots->hp;
	ownerCharacter->currentStr -= boots->str;
	ownerCharacter->currentMag -= boots->mag;
	ownerCharacter->currentDef -= boots->def;
	ownerCharacter->currentRes -= boots->res;
	ownerCharacter->currentSkill -= boots->skill;
	ownerCharacter->currentSpeed -= boots->speed;
	ownerCharacter->currentMoveSpeed -= boots->moveSpeed;
	ownerCharacter->currentAp -= boots->ap;
	ownerCharacter->damageReduction -= boots->damageReduction;
	ownerCharacter->damageReduction_Percent -= boots->damageReduction_percent;
	ownerCharacter->damageReinforcement -= boots->damageReinforcement;
	ownerCharacter->damageReinforcement_Percent -= boots->damageReinforcement_percent;
	boots->ownerCharacter = nullptr;
	boots = nullptr;
	CalcEquipmentStats();
	if (ownerCharacter)
	{
		ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
		ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
		ownerCharacter->critical = ownerCharacter->CalcCritical(0);
		ownerCharacter->SetHealthWidget();
	}
}

//스탯 상세보기 위젯에서 보여주기 위함, 실제 스탯 반영x
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
	int32 DamageReduction = 0;
	float DamageReduction_percent = 0;
	int32 DamageReinforcement = 0;
	float DamageReinforcement_percent = 0;
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
		DamageReduction += weapon->damageReduction;
		DamageReduction_percent += weapon->damageReduction_percent;
		DamageReinforcement += weapon->damageReinforcement;
		DamageReinforcement_percent += weapon->damageReinforcement_percent;
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
		DamageReduction += helmet->damageReduction;
		DamageReduction_percent += helmet->damageReduction_percent;
		DamageReinforcement += helmet->damageReinforcement;
		DamageReinforcement_percent += helmet->damageReinforcement_percent;
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
		DamageReduction += chest->damageReduction;
		DamageReduction_percent += chest->damageReduction_percent;
		DamageReinforcement += chest->damageReinforcement;
		DamageReinforcement_percent += chest->damageReinforcement_percent;
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
		DamageReduction += glove->damageReduction;
		DamageReduction_percent += glove->damageReduction_percent;
		DamageReinforcement += glove->damageReinforcement;
		DamageReinforcement_percent += glove->damageReinforcement_percent;
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
		DamageReduction += leg->damageReduction;
		DamageReduction_percent += leg->damageReduction_percent;
		DamageReinforcement += leg->damageReinforcement;
		DamageReinforcement_percent += leg->damageReinforcement_percent;
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
		DamageReduction += boots->damageReduction;
		DamageReduction_percent += boots->damageReduction_percent;
		DamageReinforcement += boots->damageReinforcement;
		DamageReinforcement_percent += boots->damageReinforcement_percent;
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
	equipmentDamageReduction = DamageReduction;
	equipmentDamageReduction_percent = DamageReduction_percent;
	equipmentDamageReinforcement = DamageReinforcement;
	equipmentDamageReinforcement_percent = DamageReinforcement_percent;
}
