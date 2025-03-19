// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widget/HealthWidget.h"
#include "Controller/CharacterController.h"
#include "Kismet/GameplayStatics.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	springArmComponent->SetupAttachment(RootComponent);
	springArmComponent->SetUsingAbsoluteRotation(true);
	springArmComponent->TargetArmLength = 2000.0f;
	springArmComponent->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
	springArmComponent->bDoCollisionTest = false;
	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetupAttachment(springArmComponent, USpringArmComponent::SocketName);
	cameraComponent->bUsePawnControlRotation = false;

	widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	widgetComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprint'/Game/Widget/W_HealthWidget'"));
	if (WidgetClass.Succeeded())
	{
		widgetComponent->SetWidgetClass(WidgetClass.Class);  // BP로 만든 위젯을 설정
	}

	SetStats();
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	playerController = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	healthWidget = Cast<UHealthWidget>(widgetComponent->GetWidget());
	if (healthWidget)
	{
		healthWidget->SetHealthBar(currentHp, hp);
		healthWidget->SetTurnText(-1);					//비워놓기, 전투 시작 시 채워야 함
		widgetComponent->SetDrawSize(FVector2D(250.0f, 30.0f));
		widgetComponent->SetWidgetSpace(EWidgetSpace::World);
		widgetComponent->SetRelativeLocation(FVector(0, 0, 200.0f));  // 캐릭터 머리 위로 배치
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = playerController->cameraRotation;
	NewRotation.Yaw += 180.0f;
	NewRotation.Pitch += 120.0f;
	widgetComponent->SetWorldRotation(NewRotation);
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::Skill1()
{
}

void ACharacterBase::Skill2()
{
}

void ACharacterBase::Skill3()
{
}

void ACharacterBase::Skill4()
{
}

void ACharacterBase::Skill5()
{
}

void ACharacterBase::ReflectDamage(int finalDamage, float acurracyRate, float criticalRate)
{
	//피공격자의 회피보다 공격자의 명중이 높게 나오면 공격 성공, 물리인지, 마법인지는 마우스에 따라다닐 위젯 컴포넌트에서 대응되는 방어스탯 가져와 계산 후 데미지만 넘겨줌, 데미지 값과 명중, 회심률에 따라 체력만 반영하면 됨
	if (FMath::RandRange(1, (int)acurracyRate) > evasion)
	{
		if (FMath::RandRange(1, 100) <= critical)
		{
			finalDamage *= 2;
		}
		currentHp -= finalDamage;
		//체력 컴포넌트 값 변경해줘야함
		if (currentHp <= 0)
		{
			this->Destroy();
			//게임모드의 큐에서 해당 캐릭터 삭제해줘야함
		}
	}
}

void ACharacterBase::GetEXP()	//일단 고정치로 몹 잡으면 무조건 같은 양의 경험치 얻도록
{
	exp += 10;
	while (exp >= 100)
	{
		exp -= 100;
		LevelUp();
	}
}

void ACharacterBase::SetLevel()
{
}

void ACharacterBase::SetStats()
{
	moveSpeed = 10;
	currentHp = hp;
	currentDef = def;
	currentMag = mag;
	currentRes = res;
	currentSkill = skill;
	currentSpeed = speed;
	currentStr = str;
}

void ACharacterBase::CalcCritical(int correction)
{
}

void ACharacterBase::CalcEvasion(int correction)
{
}

void ACharacterBase::CalcAccuracy(int correction)
{
}

void ACharacterBase::CalcDMG(int correction)
{
}

void ACharacterBase::LevelUp()
{
	const int32 MaxGrowth = 100;
	level++;
	if (FMath::RandRange(1, MaxGrowth) <= hpGrowth)
	{
		hp += 1;
	}
	if (FMath::RandRange(1, MaxGrowth) <= strGrowth)
	{
		str += 1;
	}
	if (FMath::RandRange(1, MaxGrowth) <= magGrowth)
	{
		mag += 1;
	}
	if (FMath::RandRange(1, MaxGrowth) <= defGrowth)
	{
		def += 1;
	}
	if (FMath::RandRange(1, MaxGrowth) <= resGrowth)
	{
		res += 1;
	}
	if (FMath::RandRange(1, MaxGrowth) <= skillGrowth)
	{
		skill += 1;
	}
	if (FMath::RandRange(1, MaxGrowth) <= speedGrowth)
	{
		speed += 1;
	}
}