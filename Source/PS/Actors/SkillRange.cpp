// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SkillRange.h"

#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"

// Sets default values
ASkillRange::ASkillRange()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("SkillRangeDecal"));
    RootComponent = DecalComponent;
    
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMaterialAsset(TEXT("/Game/Material/M_SkillRange"));
    if (DecalMaterialAsset.Succeeded())
    {
        DecalComponent->SetDecalMaterial(DecalMaterialAsset.Object);
    }
    // 데칼이 바닥을 향하도록 회전시킵니다. (Y축 기준 -90도 회전)
    DecalComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

    // 데칼의 크기를 설정합니다. X는 투사 깊이, Y/Z는 투사될 평면의 크기입니다.
    // 실제 반지름은 SetRadius()에서 설정하므로 여기서는 기본값만 지정합니다.
    DecalComponent->DecalSize = FVector(30.0f, 100.0f, 100.0f); // 기본값: 깊이 300, 반지름 100
}

void ASkillRange::SetRadius(float radius)
{	// 액터의 스케일을 조절하는 대신, 데칼의 크기(DecalSize)를 직접 조절합니다.
	if (DecalComponent)
	{
		// 바닥을 향하도록 회전된 데칼의 경우, DecalSize의 Y와 Z값이 반지름을 결정합니다.
		// X값(투사 깊이)은 기존 값을 유지하고 Y, Z값만 업데이트합니다.
		DecalComponent->DecalSize = FVector(radius, radius, radius);
		DecalComponent->MarkRenderStateDirty();
	}
}

float ASkillRange::GetRadius()
{
	if (DecalComponent)
	{
		return DecalComponent->DecalSize.Y;
	}
	return 0.0f;
}

// Called when the game starts or when spawned
void ASkillRange::BeginPlay()
{
	Super::BeginPlay();
	
}