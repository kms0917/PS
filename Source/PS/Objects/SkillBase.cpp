// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/SkillBase.h"
#include "Character/CharacterBase.h"

void USkillBase::ApllyDamage()
{
	for (ACharacterBase* Character : overlappedCharacters)
	{
		if (Character)
		{
			Character->ReflectDamage(bIsHeal);
		}
	}
	overlappedCharacters.Empty();
}
