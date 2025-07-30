// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SkillIndicator.h"
#include "Character/CharacterBase.h"
#include "Controller/CharacterController.h"
#include "Objects/SkillBase.h"
#include "Widget/SkillIndicatorWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASkillIndicator::ASkillIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
    OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    OverlapSphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
    OverlapSphere->SetGenerateOverlapEvents(true);
    RootComponent = OverlapSphere;

    DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("RangeDecal"));
    DecalComponent->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalMat(TEXT("/Game/Material/M_SkillIndicator"));
    if (DecalMat.Succeeded())
    {
        DecalComponent->SetDecalMaterial(DecalMat.Object);
    }
    DecalComponent->DecalSize = FVector(3.f, 3.f, 1.f); // XY = 반지름, Z = 얇게
    DecalComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f)); // Z축 아래로 → 바닥 투영
    DecalComponent->SetRelativeLocation(FVector(0.f, 0.f, 10.f));

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    WidgetComponent->SetupAttachment(RootComponent);

    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprint'/Game/Widget/W_SkillIndicatorWidget'"));
    if (WidgetClass.Succeeded())
    {
        WidgetComponent->SetWidgetClass(WidgetClass.Class);  // BP로 만든 위젯을 설정
    }
    //WidgetComponent->SetWorldScale3D(FVector(1.f));
    WidgetComponent->SetDrawAtDesiredSize(false);
    WidgetComponent->SetDrawSize(FVector2D(200, 100));
    WidgetComponent->SetRelativeLocation(FVector(-50.f, 0.f, 0.f));
    WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    WidgetComponent->SetGenerateOverlapEvents(false);

    WidgetComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    WidgetComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WidgetComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    WidgetComponent->SetReceivesDecals(false);
}

// Called when the game starts or when spawned
void ASkillIndicator::BeginPlay()
{
	Super::BeginPlay();
    
    if (OverlapSphere)
    {
        OverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &ASkillIndicator::OnSphereOverlapBegin);
        OverlapSphere->OnComponentEndOverlap.AddDynamic(this, &ASkillIndicator::OnSphereOverlapEnd);
    }
    
    playerController = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    playerCharacter = playerController->playerCharacter;

    usableWidget = Cast<USkillIndicatorWidget>(WidgetComponent->GetWidget());
    usableWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ASkillIndicator::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 함수 내용은 거의 동일합니다.
    if (!OtherActor || OtherActor == this) return;
    
    if (ACharacterBase* Casted = Cast<ACharacterBase>(OtherActor))
    {
        Casted->TargettedOn(accuracy, critical, damage, bIsMag, false, IsHeal);
        Casted->SetOverlayMaterialEnabled(true);
        overlappedCharacters.Add(Casted);
    }
}

void ASkillIndicator::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // 함수 내용은 거의 동일합니다.
    if (!OtherActor || OtherActor == this) return;

    if (ACharacterBase* Casted = Cast<ACharacterBase>(OtherActor))
    {
        if (overlappedCharacters.Contains(Casted))
        {
            overlappedCharacters.Remove(Casted);
            if (playerController && !playerController->targettedCharacter.Contains(Casted)) // playerController 유효성 검사 추가
            {
                Casted->SetOverlayMaterialEnabled(false);
                Casted->TargettedOff();
            }
        }
    }
}

void ASkillIndicator::SetSkillIndicator(int32 accuracyRate, int32 criticalRate, int32 Damage, bool isMag, float attackRange, bool isHeal)
{
    const float Scale = attackRange;

    if (DecalComponent)
    {
        DecalComponent->DecalSize = FVector(Scale, Scale, Scale);
    }
    if (OverlapSphere)
    {
        OverlapSphere->SetSphereRadius(attackRange);
    }
    IsHeal = isHeal;
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

void ASkillIndicator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator NewRotation = playerController->cameraRotation;
    NewRotation.Yaw += 180.0f;
    NewRotation.Pitch += 120.0f;
    WidgetComponent->SetWorldRotation(NewRotation);
}