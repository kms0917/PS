// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SkillIndicator.h"

// Sets default values
ASkillIndicator::ASkillIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ASkillIndicator::BeginPlay()
{
	Super::BeginPlay();
	
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

// Called every frame
void ASkillIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

