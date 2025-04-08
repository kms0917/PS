// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SkillRange.h"

// Sets default values
ASkillRange::ASkillRange()
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

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat(TEXT("/Game/Material/M_SkillRange"));
    if (Mat.Succeeded())
    {
        MeshComponent->SetMaterial(0, Mat.Object);
    }

    MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASkillRange::SetRadius(float radius)
{
    const float Scale = radius / 50.0f; // Cylinder 기본 반지름이 50이므로
    SetActorScale3D(FVector(Scale, Scale, 0.01f));
}

// Called when the game starts or when spawned
void ASkillRange::BeginPlay()
{
	Super::BeginPlay();
	
}