// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Widget/HealthWidget.h"
#include "Widget/SkillInfoWidget.h"
#include "Controller/CharacterController.h"
#include "ActorComponent/EquipmentComponent.h"
#include "ActorComponent/SkillComponent.h"
#include "Objects/SkillBase.h"

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
	springArmComponent->SetUsingAbsoluteRotation(true);
	springArmComponent->TargetArmLength = 1200.0f;
	springArmComponent->SetRelativeRotation(FRotator(-60.0f, 45.0f, 0.0f));
	springArmComponent->bDoCollisionTest = false;
	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComponent->SetupAttachment(springArmComponent, USpringArmComponent::SocketName);
	cameraComponent->bUsePawnControlRotation = false;

	healthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	healthWidgetComponent->SetupAttachment(RootComponent);
	skillInfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("skillInfoWidget"));
	//skillInfoWidgetComponent->SetupAttachment(springArmComponent);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprint'/Game/Widget/W_HealthWidget'"));
	if (WidgetClass.Succeeded())
	{
		healthWidgetComponent->SetWidgetClass(WidgetClass.Class);  // BP로 만든 위젯을 설정
		healthWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> SkillInfoWidgetBP(TEXT("WidgetBlueprint'/Game/Widget/W_SkillInfoWidget'"));
	if (SkillInfoWidgetBP.Succeeded())
	{
		skillInfoWidgetComponent->SetWidgetClass(SkillInfoWidgetBP.Class);
		skillInfoWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	equipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));
	skillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("Skills"));
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	playerController = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	healthWidget = Cast<UHealthWidget>(healthWidgetComponent->GetWidget());
	if (healthWidget)
	{
		healthWidget->SetHealthBar(currentHp, hp);
		healthWidget->SetTurnText(-1);					//비워놓기, 전투 시작 시 채워야 함
		healthWidgetComponent->SetDrawSize(FVector2D(250.0f, 30.0f));
		healthWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		healthWidgetComponent->SetRelativeLocation(FVector(0, 0, 200.0f));  // 캐릭터 머리 위로 배치
	}
	skillInfoWidget = Cast<USkillInfoWidget>(skillInfoWidgetComponent->GetWidget());
	if (skillInfoWidget)
	{
		skillInfoWidgetComponent->SetDrawSize(FVector2D(800.0f, 90.0f));
		skillInfoWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	}
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWidgetRotation();
	if (bIsTargeted)
	{
		UpdateSkillInfoWidgetLocation();
	}
}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::SetDefaultEquipments()
{
}	   //하위 클래스의 생성자에서 호출해야함

void ACharacterBase::SetDefaultSkills()
{
}	   //하위 클래스 생성자에서 호출

void ACharacterBase::UseSkill(int i)	//위젯에 연결
{
	if (skillComponent->skillList.IsValidIndex(i))
	{
		USkillBase* usedSkill = skillComponent->skillList[i];
		evasion = usedSkill->calculatedEvasion;
		critical = usedSkill->calculatedCritical;
		accuracy = usedSkill->calculatedAccuracy;
		int damage = usedSkill->calculatedDamage;
		playerController->InitSkillMode(accuracy, critical, damage, usedSkill->apUsage, usedSkill->bIsMag, usedSkill->skillRange, usedSkill->attackRange);
	}
}

void ACharacterBase::ReflectDamage(int Damage, float acurracyRate, float criticalRate, bool bIsMag)
{
	//피공격자의 회피보다 공격자의 명중이 높게 나오면 공격 성공, 물리인지, 마법인지는 마우스에 따라다닐 위젯 컴포넌트에서 대응되는 방어스탯 가져와 계산 후 데미지만 넘겨줌, 데미지 값과 명중, 회심률에 따라 체력만 반영하면 됨
	if (FMath::RandRange(1, (int)acurracyRate) > evasion)
	{
		if (bIsMag)
		{
			Damage -= res;
		}
		else
		{
			Damage -= def;
		}
		if (FMath::RandRange(1, 100) <= criticalRate)
		{
			Damage *= 2;
		}
		currentHp -= Damage;
		//체력 컴포넌트 값 변경해줘야함
		if (currentHp <= 0)
		{
			this->Destroy();
			//게임모드의 큐에서 해당 캐릭터 삭제해줘야함
		}
	}
	else
	{
		//회피시의 로직 필요
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

void ACharacterBase::SetLevel()		//적들의 레벨 스케일링에 사용될 함수, 게임모드 통해 플레이어 캐릭터 레벨 받아와서 레벨 계산
{
}

void ACharacterBase::UpdateWidgetRotation()
{
	FRotator NewRotation = playerController->cameraRotation;
	NewRotation.Yaw += 180.0f;
	NewRotation.Pitch += 120.0f;
	healthWidgetComponent->SetWorldRotation(NewRotation);
	skillInfoWidgetComponent->SetWorldRotation(NewRotation);
}

void ACharacterBase::UpdateSkillInfoWidgetLocation()
{
	if (!skillInfoWidgetComponent || !playerController) return;

	FVector CharacterLocation = GetActorLocation();
	const FRotator& CameraRot = playerController->cameraRotation;

	FVector CameraRight = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::Y);
	FVector CameraForward = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::X);

	const float RightOffset = 550.0f;
	const float ForwardOffset = -60.0f;

	FVector Offset = CameraForward * ForwardOffset + CameraRight * RightOffset;


	FVector WidgetWorldLocation = CharacterLocation + Offset;
	skillInfoWidgetComponent->SetWorldLocation(WidgetWorldLocation);
}

int ACharacterBase::CalcDamage(int damage, float magnification, bool isMag)
{
	if (!isMag)			//물리 딜
	{
		return damage + str * magnification + equipmentComponent->equipmentDamage;
	}
	else				//마법 딜
	{
		return damage + mag * magnification + equipmentComponent->equipmentDamage;
	}
}

void ACharacterBase::SetStats()		//매 턴 개시 및 하위 클래스 생성자에서 호출, 추후 여러 턴에 걸쳐 지속되는 버프 만들 시 수정 필요
{
	currentMoveSpeed = moveSpeed + equipmentComponent->equipmentMoveSpeed;
	currentHp = hp + equipmentComponent->equipmentHp;
	currentDef = def + equipmentComponent->equipmentDef;
	currentMag = mag + equipmentComponent->equipmentMag;
	currentRes = res + equipmentComponent->equipmentRes;
	currentSkill = skill + equipmentComponent->equipmentSkill;
	currentSpeed = speed + equipmentComponent->equipmentSpeed;
	currentStr = str + equipmentComponent->equipmentStr;
	currentAp = ap + equipmentComponent->equipmentAp;
	critical = CalcCritical(0);	//이 3종의 함수는 스킬의 추가 보정값이 없는경우 호출x, 있을때만 스킬에서 추가로 호출해서 스킬의 보정값 사용함
	evasion = CalcEvasion(0);
	accuracy = CalcAccuracy(0);
}

void ACharacterBase::SetSkillInfo()
{
	TArray<USkillBase*> skills = skillComponent->skillList;

	for (int i = 0; i < skills.Num(); i++)
	{
		skills[i]->calculatedDamage = CalcDamage(skills[i]->damage, skills[i]->magnification, skills[i]->bIsMag);
		skills[i]->calculatedAccuracy = CalcAccuracy(skills[i]->accuracy);
		skills[i]->calculatedCritical = CalcCritical(skills[i]->critical);
		skills[i]->calculatedEvasion = CalcEvasion(skills[i]->evasion);
	}
}

int ACharacterBase::CalcCritical(int correction)
{
	return (skill + equipmentComponent->equipmentCritical + correction);
}

int ACharacterBase::CalcEvasion(int correction)
{
	return (speed * 1.2 + equipmentComponent->equipmentEvasion + correction);
}

int ACharacterBase::CalcAccuracy(int correction)
{
	return (skill * 1.2 + equipmentComponent->equipmentAccuracy + correction);
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