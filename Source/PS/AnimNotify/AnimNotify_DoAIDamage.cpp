// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_DoAIDamage.h"
#include "Character/CharacterBase.h"
#include "Objects/SkillBase.h"
#include "Controller/AIController/BasicAIController.h"
#include "Actors/ProjectileBase.h"

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
        USkillBase* selectedSkill =  AICon->GetSelectedSkill();
        if (selectedSkill->projectile)     //투사체가 있는 경우, 스킬 이펙트 출력은 없어야하고 투사체만 발사, 투사체가 사라질때 투사체가 가진 이펙트 출력하며 위의 로직 그대로 따라가야함
        {
            TSubclassOf<AProjectileBase> ProjectileClass = selectedSkill->projectile;
            UWorld* World = Character->GetWorld();
            FVector SpawnLocation = Character->GetActorLocation();
            //attackRangeIndicator로 발사체 발사
            AActor* targetActor = AICon->GetTargetCharacter();
            if (targetActor)
            {
                FRotator SpawnRotation = (targetActor->GetActorLocation() - SpawnLocation).Rotation();
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = Character;
                SpawnParams.Instigator = Character;
                // 캐릭터와 겹쳐서 스폰 실패하는 것을 방지
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                AProjectileBase* SpawnedProjectile = World->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
                {
                    SpawnedProjectile->SetProjectile(Character->GetController(), Character->currentUsedSkill, targetActor);
                }
            }
        }
        else
        {
            AICon->DoAIDamage();
        }
    }
}
