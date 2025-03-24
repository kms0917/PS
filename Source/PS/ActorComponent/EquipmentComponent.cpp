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
}

void UEquipmentComponent::EquipWeapon(UWeaponBase* EquipedWeapon)
{
	UnEquipWeapon();
	weapon = EquipedWeapon; 
	if (ownerCharacter)
	{
		weapon->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::EquipHelmet(UHelmetBase* EquipedHelmet)
{
	UnEquipHelmet();
	helmet = EquipedHelmet;
	if (ownerCharacter)
	{
		helmet->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::EquipChest(UChestBase* EquipedChest)
{
	UnEquipChest();
	chest = EquipedChest;
	if (ownerCharacter)
	{
		chest->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::EquipGlove(UGloveBase* EquipedGlove)
{
	UnEquipGlove();
	glove = EquipedGlove;
	if (ownerCharacter)
	{
		glove->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::EquipLeg(ULegBase* EquipedLeg)
{
	UnEquipLeg();
	leg = EquipedLeg;
	if (ownerCharacter)
	{
		leg->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::EquipBoots(UBootsBase* EquipedBoots)
{
	UnEquipBoots();
	boots = EquipedBoots;
	if (ownerCharacter)
	{
		boots->ownerCharacter = ownerCharacter;
	}
}

void UEquipmentComponent::UnEquipWeapon()
{
	weapon->ownerCharacter = nullptr;
	weapon = nullptr;
}

void UEquipmentComponent::UnEquipHelmet()
{
	helmet->ownerCharacter = nullptr;
	helmet = nullptr;
}

void UEquipmentComponent::UnEquipChest()
{
	chest->ownerCharacter = nullptr;
	chest = nullptr;
}

void UEquipmentComponent::UnEquipGlove()
{
	glove->ownerCharacter = nullptr;
	glove = nullptr;
}

void UEquipmentComponent::UnEquipLeg()
{
	leg->ownerCharacter = nullptr;
	leg = nullptr;
}

void UEquipmentComponent::UnEquipBoots()
{
	boots->ownerCharacter = nullptr;
	boots = nullptr;
}
