// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/SkillBase.h"

#include "NiagaraFunctionLibrary.h"
#include "Character/CharacterBase.h"
#include "Objects/Buff/BuffBase.h"
#include "ActorComponent/BuffComponent.h"
#include "Controller/CharacterController.h"
#include "Widget/SkillWidget.h"

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
				Character->playerController->skillWidgetInstance->UpdateButtons(Character->currentAp);
			}
			if (skillEffectParticle)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), skillEffectParticle, Character->GetActorLocation(), Character->GetActorRotation());
			}
		}
	}
	overlappedCharacters.Empty();
}
