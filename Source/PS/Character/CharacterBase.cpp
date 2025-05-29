// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

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

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));
	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass()); // 시야 감지 등록
	StimuliSource->bAutoRegister = true;
}

//위젯 및 플레이어 컨트롤러 세팅
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
		skillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
		skillInfoWidgetComponent->SetDrawSize(FVector2D(1000.0f, 120.0f));
		skillInfoWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	}
}

//위젯 가시성 조절
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWidgetRotation();
	if (bIsTargeted)
	{
		UpdateSkillInfoWidgetLocation();
	}
}

//플레이어 컨트롤러에서만 호출
void ACharacterBase::OnSkillAutoMoveFinished(FVector attackPoint)
{
	if (currentUsedSkill && currentUsedSkill->skillMontage)
	{
		FVector Direction = attackPoint - GetActorLocation();
		Direction.Z = 0.0f; // 수평 회전만 고려
		if (!Direction.IsNearlyZero())
		{
			FRotator NewRotation = Direction.Rotation();
			SetActorRotation(NewRotation);
		}

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(currentUsedSkill->skillMontage, currentUsedSkill->PlayRate);
		}
	}
}

//플레이어 컨트롤러에서 입력 막기위해 몽타뉴 플레이중인지 반환
bool ACharacterBase::IsMontagePlayed()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		return AnimInstance->IsAnyMontagePlaying();
	}
	return false;
}

//하위 클래스의 생성자에서 오버라이드해서 호출
void ACharacterBase::SetDefaultEquipments()
{
}

//하위 클래스의 생성자에서 오버라이드해서 호출
void ACharacterBase::SetDefaultSkills()
{
}	   

//플레이어 컨트롤러 주도하에 위젯에서 호출됨
void ACharacterBase::UseSkill(int i)	
{
	if (skillComponent->skillList.IsValidIndex(i))
	{
		USkillBase* usedSkill = skillComponent->skillList[i];
		currentUsedSkill = usedSkill;
		evasion = usedSkill->calculatedEvasion;
		critical = usedSkill->calculatedCritical;
		accuracy = usedSkill->calculatedAccuracy;
		int damage = usedSkill->calculatedDamage;
		playerController->InitSkillMode(accuracy, critical, damage, usedSkill->apUsage, usedSkill->bIsMag, usedSkill->skillRange, usedSkill->attackRange);
	}
}

//스킬 사용시의 AnimNotify 통해 targetted 된 객체들의 함수를 사용되는 skill object에서 호출
void ACharacterBase::ReflectDamage()
{
	if (FMath::RandRange(1, 100) <= savedAccuracy)
	{
		if (FMath::RandRange(1, 100) <= savedCritical)
		{
			savedDamage *= 2;
		}
		currentHp -= savedDamage;
		healthWidget->SetHealthBar(currentHp, hp);
		if (currentHp <= 0)
		{
			this->Destroy();
			//게임모드의 큐에서 해당 캐릭터 삭제해줘야함
			//게임모드에서 아군 캐릭터들의 목록 소유하고 있어야함, 게임모드에서 아군 캐릭터들의 경험치 습득함수 작동시켜줘야함
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("evade!"));
		//회피시의 로직 필요
	}
	savedAccuracy = 0;
	savedDamage = 0;
	savedCritical = 0;	//부자연스러우면 targetedOff에서 실행, 조건달아서 현재 캐릭터의 currentUsedSkill의 overlappedCharacter 확인해서 분기
}

//얻는 수치는 조절 필요, 적이 죽을때 게임모드에서 호출해서 모든 아군 캐릭터 경험치 습득해줘야함
void ACharacterBase::GetEXP()	
{
	exp += 10;
	while (exp >= 100)
	{
		exp -= 100;
		LevelUp();
	}
}

//적들의 레벨 스케일링에 사용될 함수, 게임모드 통해 플레이어 캐릭터 레벨 받아와서 레벨 계산
void ACharacterBase::SetLevel(int32 levelScaleAmount)
{
	while (level == levelScaleAmount)
	{
		LevelUp();
	}
}

//skillIndicator와 오버렙 시 호출
void ACharacterBase::TargettedOn(int32 accuracyRate, int32 criticalRate, int32 Damage, bool isMag)
{
	if (isMag)	//마딜이면
	{
		Damage -= res;
	}
	else
	{
		Damage -= def;
	}
	savedAccuracy = (((float)accuracyRate - (float)evasion) / (float)accuracyRate) * 100;
	savedDamage = Damage;
	savedCritical = criticalRate;

	skillInfoWidget->SettingWidget(savedAccuracy, criticalRate, Damage);
	skillInfoWidget->SetVisibility(ESlateVisibility::Visible);
}

//skillIndicator와 오버렙 끝날시 호출
void ACharacterBase::TargettedOff()
{
	//savedAccuracy = 0;
	//savedDamage = 0;
	//savedCritical = 0;

	skillInfoWidget->RessetWidget();
	skillInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
}

//위젯 각도조절
void ACharacterBase::UpdateWidgetRotation()
{
	FRotator NewRotation = playerController->cameraRotation;
	NewRotation.Yaw += 180.0f;
	NewRotation.Pitch += 120.0f;
	healthWidgetComponent->SetWorldRotation(NewRotation);
	skillInfoWidgetComponent->SetWorldRotation(NewRotation);
}

//Targetted 됐을 시 띄울 위젯 컴포넌트 위치 조절
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

//skillToolTip에 띄울 값 계산
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

//자원 및 스탯 초기화, 매 턴 개시 및 하위 클래스 생성자에서 호출, 추후 여러 턴에 걸쳐 지속되는 버프 만들 시 수정 필요
void ACharacterBase::SetStats()		
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

//스킬들의 내부 값들을 미리 계산, 하위 클래스의 생성자와 턴 개시 시 호출해야함
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

//치명타 확률 계산, 위의 함수들에서 사용
int ACharacterBase::CalcCritical(int correction)
{
	return (skill + equipmentComponent->equipmentCritical + correction);
}

//회피율 계산, 위의 함수들에서 사용
int ACharacterBase::CalcEvasion(int correction)
{
	return (speed * 1.2 + equipmentComponent->equipmentEvasion + correction);
}

//명중률 계산, 위의 함수들에서 사용
int ACharacterBase::CalcAccuracy(int correction)
{
	return (skill * 1.2 + equipmentComponent->equipmentAccuracy + correction);
}

//레벨업, 경험치 얻는 함수에서 사용
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