// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_DoAIDamage.h"
#include "Character/CharacterBase.h"
#include "Objects/SkillBase.h"
#include "Controller/AIController/BasicAIController.h"

void UAnimNotify_DoAIDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    ACharacterBase* Character = Cast<ACharacterBase>(Owner);
    if (!Character) return;

    ABasicAIController* AICon = Cast<ABasicAIController>(Character->GetController());
    if (AICon)
    {
        AICon->DoAIDamage();
    }
}
