// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_DealDamage.h"
#include "Character/CharacterBase.h"
#include "Objects/SkillBase.h"

void UAnimNotify_DealDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    ACharacterBase* Character = Cast<ACharacterBase>(Owner);
    if (!Character) return;

    if (Character->currentUsedSkill)
    {
        Character->currentUsedSkill->ApllyDamage();
        Character->bDidApplyDamage = true;
        Character->currentUsedSkill = nullptr;
    }
}
