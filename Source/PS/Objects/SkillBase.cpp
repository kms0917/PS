// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/SkillBase.h"
#include "Character/CharacterBase.h"
#include "Objects/Buff/BuffBase.h"
#include "ActorComponent/BuffComponent.h"

void USkillBase::ApllyDamage()
{
	for (ACharacterBase* Character : overlappedCharacters)
	{
		if (Character)
		{
			Character->ReflectDamage(bIsHeal);
			if (buff)
			{
				Character->buffComponent->GetBuff(buff);
			}
		}
	}
	overlappedCharacters.Empty();
}
