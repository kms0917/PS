// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AnimNotify_DealDamage.h"
#include "Character/CharacterBase.h"
#include "Objects/SkillBase.h"
#include "Controller/CharacterController.h"
#include "Actors/ProjectileBase.h"
#include "Actors/SkillIndicator.h"

void UAnimNotify_DealDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp) return;

    AActor* Owner = MeshComp->GetOwner();
    if (!Owner) return;

    ACharacterBase* Character = Cast<ACharacterBase>(Owner);
    if (!Character) return;
    if (Character->GetGenericTeamId() == FGenericTeamId(1)) return;
    
    if (Character->currentUsedSkill->projectile)     //투사체가 있는 경우, 스킬 이펙트 출력은 없어야하고 투사체만 발사, 투사체가 사라질때 투사체가 가진 이펙트 출력하며 위의 로직 그대로 따라가야함
    {
        TSubclassOf<AProjectileBase> ProjectileClass = Character->currentUsedSkill->projectile;
        UWorld* World = Character->GetWorld();
        FVector SpawnLocation = Character->GetActorLocation();
        
        if (Character->currentUsedSkill->multiTargetingNum > 1)		//다중 타겟팅 스킬인 경우, InitAttack으로 Skill에 대상 리스트 넘긴 후 순회하며 발사체 발사
        {
            Character->playerController->InitAttack();
            for (int i = 0; i < Character->currentUsedSkill->overlappedCharacters.Num(); i++)
            {
                AActor* targetActor = Cast<AActor>(Character->currentUsedSkill->overlappedCharacters[i]);
                if (targetActor)
                {
                    //발사체 생성 후 발사
                    FRotator SpawnRotation = (targetActor->GetActorLocation() - SpawnLocation).Rotation();
                    FActorSpawnParameters SpawnParams;
                    SpawnParams.Owner = Character;
                    SpawnParams.Instigator = Character;
                    // 캐릭터와 겹쳐서 스폰 실패하는 것을 방지
                    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
                    AProjectileBase* SpawnedProjectile = World->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
                    {
                        SpawnedProjectile->SetProjectile(Character->playerController, Character->currentUsedSkill, targetActor);
                    }
                }
            }
        }
        else
        {
            //attackRangeIndicator로 발사체 발사
            AActor* targetActor = Cast<AActor>(Character->playerController->attackRangeIndicator);
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
                    SpawnedProjectile->SetProjectile(Character->playerController, Character->currentUsedSkill, targetActor);
                }
            }
        }
    }
    else if (Character->currentUsedSkill)    //투사체가 없는 경우, 이펙트 출력해야 함
    {
        Character->playerController->InitAttack();
        Character->currentUsedSkill->ApllyDamage();
        Character->currentUsedSkill = nullptr;
    }
}
