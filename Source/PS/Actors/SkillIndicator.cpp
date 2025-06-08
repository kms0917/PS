// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SkillIndicator.h"
#include "Character/CharacterBase.h"
#include "Controller/CharacterController.h"
#include "Objects/SkillBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASkillIndicator::ASkillIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = MeshComponent;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> DiskMesh(TEXT("/Engine/BasicShapes/Cylinder"));
    if (DiskMesh.Succeeded())
    {
        MeshComponent->SetStaticMesh(DiskMesh.Object);
    }
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat(TEXT("/Game/Material/M_SkillIndicator"));
    if (Mat.Succeeded())
    {
        MeshComponent->SetMaterial(0, Mat.Object);
    }

    MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    MeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void ASkillIndicator::BeginPlay()
{
	Super::BeginPlay();
	
    OnActorBeginOverlap.AddDynamic(this, &ASkillIndicator::OverlapWithCharacter);
    OnActorEndOverlap.AddDynamic(this, &ASkillIndicator::OverlapEnd);

    playerCharacter = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->playerCharacter;
}

void ASkillIndicator::OverlapWithCharacter(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor || OtherActor == this) return;
    
    if (ACharacterBase* Casted = Cast<ACharacterBase>(OtherActor))
    {
        Casted->TargettedOn(accuracy, critical, damage, bIsMag, false);
        overlappedCharacters.Add(Casted);
    }
}

void ASkillIndicator::OverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor || OtherActor == this) return;

    if (ACharacterBase* Casted = Cast<ACharacterBase>(OtherActor))
    {
        if (overlappedCharacters.Contains(Casted))
        {
            overlappedCharacters.Remove(Casted);
            Casted->TargettedOff();
        }
    }
}

void ASkillIndicator::SetSkillIndicator(int32 accuracyRate, int32 criticalRate, int32 Damage, bool isMag, float attackRange)
{
    const float Scale = attackRange / 50.0f;
    SetActorScale3D(FVector(Scale, Scale, 0.01f));

    accuracy = accuracyRate;
    critical = criticalRate;
    damage = Damage;
    bIsMag = isMag;
}

void ASkillIndicator::InitAttack()
{
    if (overlappedCharacters.Num() > 0)
    {
        for (int32 i = overlappedCharacters.Num() - 1; i >= 0; --i)
        {
            ACharacterBase* Character = overlappedCharacters[i];
            if (IsValid(Character)) // 유효성 검사 (optional, safety)
            {
                playerCharacter->currentUsedSkill->overlappedCharacters.Add(Character);

                // ReflectDamage 내부에서 Destroy()가 호출될 수 있으므로,
                // 후처리로 Remove도 고려 가능
            }
        }
    }
}

void ASkillIndicator::SetUnvisible()
{
    if (overlappedCharacters.Num() > 0)
    {
        for (int32 i = overlappedCharacters.Num() - 1; i >= 0; --i)
        {
            ACharacterBase* Character = overlappedCharacters[i];
            if (IsValid(Character))
            {
                Character->TargettedOff();
            }
        }
    }
}
