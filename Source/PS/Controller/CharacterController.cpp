// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Character/CharacterBase.h"
#include "GameMode/NormalGameMode.h"
#include "Widget/SkillWidget.h"
#include "Widget/SkillInfoWidget.h"
#include "Actors/SkillRange.h"
#include "Actors/SkillIndicator.h"
#include "Actors/MovePoint.h"


ACharacterController::ACharacterController()
{
	bShowMouseCursor = true; // 마우스 커서 표시
	DefaultMouseCursor = EMouseCursor::Default;
    ClickAction = nullptr; 
    CameraMoveSpeed = 1000.0f;
    CameraRotateSpeed = 50.0f;
    
    SetBPs();
}

void ACharacterController::OnTurnChanged()  //턴이 왔을때 실행시킬 함수, 행동력 회복 등의 로직, 위젯 내용 갱신도 해야함
{
    playerCharacter = Cast<ACharacterBase>(GetPawn());      //빙의 캐릭터가 바뀐 후 호출되어야 함, 게임모드에서 관리, 적 캐릭터면 기능 다 잠궈야 함
    
}

void ACharacterController::InitSkillMode(int32 accuracy, int32 critical, int32 damage, int32 apUsage, bool isMag, float skillRange, float attackRange)
{
    if (bIsStop)
    {
        savedAp = apUsage;
        savedSkillRange = skillRange;
        bIsSkillMode = true;
        if (SkillRangeClass)
        {
            FVector spawnLocation = playerCharacter->GetActorLocation();
            spawnLocation.Z -= 90.0f;
            if (skillRangeIndicator)
            {
                skillRangeIndicator->Destroy();
            }
            skillRangeIndicator = GetWorld()->SpawnActor<ASkillRange>(SkillRangeClass, spawnLocation, FRotator::ZeroRotator);
            if (skillRangeIndicator)
            {
                skillRangeIndicator->SetRadius(skillRange);
            }
        }
        if (AttackRangeClass)
        {
            FHitResult HitResult;
            GetHitResultUnderCursor(ECC_WorldStatic, false, HitResult);
            FVector spawnpoint = HitResult.ImpactPoint;
            if (attackRangeIndicator)
            {
                attackRangeIndicator->Destroy();
            }
            attackRangeIndicator = GetWorld()->SpawnActor<ASkillIndicator>(AttackRangeClass, spawnpoint, FRotator::ZeroRotator);
            if (attackRangeIndicator)
            {
                attackRangeIndicator->SetSkillIndicator(accuracy, critical, damage, isMag, attackRange);        //공격범위 표시 후 tick에서 마우스 트래킹 및 스킬 사용 여부 판별
            }
        }
    }
}

void ACharacterController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (DefaultMappingContext)
        {
            InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
    if (TargetIndicatorClass)
    {
        targetIndicator = GetWorld()->SpawnActor<AMovePoint>(TargetIndicatorClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (targetIndicator)
        {
            targetIndicator->SetActorHiddenInGame(true); //처음엔 숨김
        }
    }
    //이 밑부분들은 턴 개시시마다 실행되어야 함
    if (APawn* ControlledPawn = GetPawn())
    {
        SpringArmComponent = ControlledPawn->FindComponentByClass<USpringArmComponent>();
        playerCharacter = Cast<ACharacterBase>(ControlledPawn);
    }
    gameMode = Cast<ANormalGameMode>(UGameplayStatics::GetGameMode(this));
    ResetCamera();
    if (skillWidgetClass)
    {
        UUserWidget* widget = CreateWidget<UUserWidget>(this, skillWidgetClass);
        skillWidgetInstance = Cast<USkillWidget>(widget);
        if (skillWidgetInstance)
        {
            skillWidgetInstance->UpdateWidget(playerCharacter);     //턴이 바뀔때마다 실행되야함
            skillWidgetInstance->AddToViewport();
        }
    }
}

void ACharacterController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInput->BindAction(ClickAction, ETriggerEvent::Triggered, this, &ACharacterController::OnRightClick);
        EnhancedInput->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &ACharacterController::MoveCamera);
        EnhancedInput->BindAction(RotateCameraAction, ETriggerEvent::Triggered, this, &ACharacterController::RotateCamera);
        EnhancedInput->BindAction(ResetCameraAction, ETriggerEvent::Triggered, this, &ACharacterController::ResetCamera);
        EnhancedInput->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &ACharacterController::ZoomCamera);
    }
}

void ACharacterController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!playerCharacter || !SpringArmComponent) return;

    CheckCameraAttachtoCharacter();     //카메라 이동 보조
    UpdateCameraRotation();         //위젯 각도 조절 위한 값 저장
    CheckCharacterMove();           //멈춰있는지 여부에 따라 flag 변경해 targetIndicator에서 위젯 표시 제어
    if (!bIsSkillMode && playerCharacter && bIsStop)
    {
        UpdateMouseCursorLocation();
    }
    else if (bIsSkillMode && playerCharacter)
    {
        UpdateSkillIndicatorLocation();
    }
    SetNavPath();                   //targetIndicator까지의 경로 표시
}

void ACharacterController::OnRightClick()
{
    if (bIsSkillMode)
    {
        StopSkillMode();
    }
    else
    {
        MoveToMouseCursor();
    }
}

void ACharacterController::StopSkillMode()
{
    bIsSkillMode = false; 
    savedAp = -1;
    savedSkillRange = -1;
    if (skillRangeIndicator)
    {
        skillRangeIndicator->Destroy();
    }
    if (attackRangeIndicator)
    {
        attackRangeIndicator->Destroy();
    }
}

void ACharacterController::MoveToMouseCursor()
{
    if (bIsStop)
    {
        FHitResult HitResult;
        GetHitResultUnderCursor(ECC_WorldStatic, false, HitResult);
        if (HitResult.bBlockingHit)
        {
            if (gameMode->bIsBattle && stopPoint != FVector::ZeroVector)  // 배틀 모드일 때 이동 거리 제한 적용
            {
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, stopPoint);
                playerCharacter->currentMoveSpeed = 0;

            }
            else  // 일반 모드에서는 마우스 클릭 위치로 바로 이동
            {
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitResult.ImpactPoint);
                if (gameMode->bIsBattle)
                {
                    playerCharacter->currentMoveSpeed -= totalDistance;
                }
            }
        }
    }
}

void ACharacterController::UpdateMouseCursorLocation()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

    if (HitResult.bBlockingHit)
    {
        FVector TargetLocation = HitResult.ImpactPoint;
        targetIndicator->SetActorLocation(TargetLocation);
    }
}

void ACharacterController::UpdateSkillIndicatorLocation()
{
    if (!attackRangeIndicator || !targetIndicator || !playerCharacter) return;

    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
    if (!HitResult.bBlockingHit) return;

    FVector MouseLocation = HitResult.ImpactPoint;
    attackRangeIndicator->SetActorLocation(MouseLocation);

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
    if (!NavSystem) return;

    FVector CharacterLocation = playerCharacter->GetActorLocation();
    const float SearchRadius = savedSkillRange;

    // Fibonacci Sphere Sampling
    TArray<FVector> Samples;
    const int NumSamples = 100;
    float Offset = 2.f / NumSamples;
    float Increment = PI * (3.f - FMath::Sqrt(5.f));

    for (int i = 0; i < NumSamples; ++i)
    {
        float Y = ((i * Offset) - 1.f) + (Offset / 2.f);
        float Radius = FMath::Sqrt(1.f - Y * Y);
        float Theta = i * Increment;

        float X = FMath::Cos(Theta) * Radius;
        float Z = FMath::Sin(Theta) * Radius;

        FVector Sample = FVector(X, Y, Z) * SearchRadius + MouseLocation;
        Samples.Add(Sample);
    }

    FNavLocation BestLocation;
    float ShortestPath = TNumericLimits<float>::Max();
    bool bFound = false;

    for (const FVector& Sample : Samples)
    {
        FNavLocation ProjectedPoint;
        if (!NavSystem->ProjectPointToNavigation(Sample, ProjectedPoint)) continue;

        // 장애물 체크
        FHitResult ObstacleHit;
        FCollisionQueryParams TraceParams(FName("SkillObstacleTrace"), true, this);
        TraceParams.AddIgnoredActor(playerCharacter);
        TraceParams.AddIgnoredActor(targetIndicator);

        bool bBlocked = GetWorld()->LineTraceSingleByChannel(
            ObstacleHit,
            ProjectedPoint.Location + FVector(0, 0, 3),
            MouseLocation + FVector(0, 0, 3),
            ECC_Visibility,
            TraceParams
        );

        if (bBlocked) continue;

        UNavigationPath* Path = NavSystem->FindPathToLocationSynchronously(
            this, CharacterLocation, ProjectedPoint.Location);

        if (Path && Path->IsValid() && Path->PathPoints.Num() > 1)
        {
            float PathLength = 0.f;
            for (int i = 1; i < Path->PathPoints.Num(); ++i)
            {
                PathLength += FVector::Dist(Path->PathPoints[i - 1], Path->PathPoints[i]);
            }

            if (PathLength < ShortestPath)
            {
                ShortestPath = PathLength;
                BestLocation = ProjectedPoint;
                bFound = true;
            }
        }
    }

    if (bFound)
    {
        targetIndicator->SetActorLocation(BestLocation.Location);
        targetIndicator->SetActorHiddenInGame(false);
    }
    else
    {
        targetIndicator->SetActorHiddenInGame(true);
    }
}


void ACharacterController::ResetCamera()
{
    if (!SpringArmComponent) return;

    FVector CharacterLocation = playerCharacter->GetActorLocation();
    SpringArmComponent->SetWorldLocation(CharacterLocation);
}

void ACharacterController::RotateCamera(const FInputActionValue& Value)
{
    if (!SpringArmComponent) return;

    float RotateValue = Value.Get<float>();

    FRotator NewRotation = SpringArmComponent->GetComponentRotation();

    NewRotation.Yaw += RotateValue * CameraRotateSpeed * GetWorld()->GetDeltaSeconds();

    SpringArmComponent->SetWorldRotation(NewRotation);
}

void ACharacterController::MoveCamera(const FInputActionValue& Value)
{
    if (!SpringArmComponent || !playerCharacter || !bCanMoveCamera) return;

    FVector2D MovementVector = Value.Get<FVector2D>();

    FVector Forward = SpringArmComponent->GetForwardVector();
    FVector Right = SpringArmComponent->GetRightVector();

    Forward.Z = 0.0f;
    Right.Z = 0.0f;
    Forward.Normalize();
    Right.Normalize();

    FVector MoveDirection = (Forward * MovementVector.Y + Right * MovementVector.X)
        * CameraMoveSpeed * GetWorld()->GetDeltaSeconds();

    SpringArmComponent->SetWorldLocation(SpringArmComponent->GetComponentLocation() + MoveDirection);
}

void ACharacterController::ZoomCamera(const FInputActionValue& Value)
{
    if (SpringArmComponent)
    {
        float AxisValue = Value.Get<float>();

        float NewTargetArmLength = SpringArmComponent->TargetArmLength + (AxisValue * ZoomSpeed);

        NewTargetArmLength = FMath::Clamp(NewTargetArmLength, MinZoomDistance, MaxZoomDistance);

        SpringArmComponent->TargetArmLength = NewTargetArmLength;
    }
}

void ACharacterController::UpdateCameraRotation()
{
    cameraRotation = SpringArmComponent->GetComponentRotation();
}

void ACharacterController::CheckCameraAttachtoCharacter()
{
    FVector CharacterLocation = playerCharacter->GetActorLocation();  // 캐릭터의 위치
    FVector SpringArmLocation = SpringArmComponent->GetComponentLocation();  // 스프링 암의 위치
    if (!IsInputKeyDown(EKeys::W) && !IsInputKeyDown(EKeys::A) && !IsInputKeyDown(EKeys::S) && !IsInputKeyDown(EKeys::D)
        && FMath::IsNearlyEqual(CharacterLocation.X, SpringArmLocation.X, 15.0f)
        && FMath::IsNearlyEqual(CharacterLocation.Y, SpringArmLocation.Y, 15.0f))
    {
        SpringArmComponent->SetRelativeLocation(CharacterLocation);
    }
    //카메라 최대 이동 제어
    float Distance = FVector::Dist(SpringArmLocation, CharacterLocation);
    if (Distance <= MaxDistance)
    {
        bCanMoveCamera = true;
    }
    else
    {
        bCanMoveCamera = false;

        FVector Direction = (SpringArmLocation - CharacterLocation).GetSafeNormal();
        FVector TargetLocation = CharacterLocation + Direction * (MaxDistance - 10.0f);
        SpringArmComponent->SetWorldLocation(TargetLocation);
    }
}

void ACharacterController::CheckCharacterMove()
{
    if (playerCharacter && playerCharacter->GetVelocity().SizeSquared() <= 0.0f)
    {
        targetIndicator->SetActorHiddenInGame(false);
        bIsStop = true;
    }
    else
    {
        targetIndicator->SetActorHiddenInGame(true);
        bIsStop = false;
    }
}

void ACharacterController::SetNavPath()
{
    if (targetIndicator && bIsStop)
    {
        FVector CharacterLocation = playerCharacter->GetActorLocation();
        UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
        FNavLocation NavLocation;
        if (NavSystem->ProjectPointToNavigation(targetIndicator->GetActorLocation(), NavLocation))
        {
            targetIndicator->HideReachableText();
            // 네비메시 경로 계산
            UNavigationPath* NavPath = NavSystem->FindPathToLocationSynchronously(
                this, CharacterLocation, NavLocation.Location);

            if (NavPath && NavPath->IsValid())
            {
                float maxMoveDistance = playerCharacter ? playerCharacter->currentMoveSpeed * 100.0f : 0.0f;  // cm 변환
                float totalPathDistance = 0.0f;
                bool reachedLimit = false;

                FVector previousEnd = NavPath->PathPoints[0]; // 첫 지점을 기준으로 설정
                stopPoint = FVector::ZeroVector;

                FColor CylinderColor = FColor::White; // 기본 흰색

                for (int32 i = 1; i < NavPath->PathPoints.Num(); i++)
                {
                    FVector Start = previousEnd;
                    FVector End = NavPath->PathPoints[i];
                    float segmentDistance = FVector::Dist(Start, End);

                    if (gameMode->bIsBattle) // 전투 모드일 때만 제한 적용
                    {
                        if (!reachedLimit && totalPathDistance + segmentDistance > maxMoveDistance)
                        {
                            float remainingDistance = maxMoveDistance - totalPathDistance;
                            FVector Direction = (End - Start).GetSafeNormal();
                            stopPoint = Start + Direction * remainingDistance; // 🚀 stopPoint 저장
                            // 이동 가능한 거리까지 흰색으로 표시
                            DrawDebugCylinder(GetWorld(), Start, stopPoint, 10.0f, 12, FColor::White, false, -1, 0, 1);
                            // 초과 부분을 빨간색으로 표시
                            Start = stopPoint;
                            if (targetIndicator)
                            {
                                //targetIndicator->SetActorLocation(stopPoint + FVector(0, 0, 0));
                            }
                            CylinderColor = FColor::Red;
                            reachedLimit = true;
                        }
                    }
                    // 초과한 구간은 계속 빨간색으로 유지
                    DrawDebugCylinder(GetWorld(), Start, End, 10.0f, 12, CylinderColor, false, -1, 0, 1);

                    totalPathDistance += segmentDistance;
                    previousEnd = End; // 이전 끝점을 갱신
                }

                totalDistance = totalPathDistance / 100.0f; // 미터 단위 변환

                if (playerCharacter)
                {
                    bIsReachable = (playerCharacter->currentMoveSpeed >= totalDistance);
                }
            }
        }
        else
        {
            targetIndicator->SetReachableText();
        }
    }
}

void ACharacterController::SetBPs()
{
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContextFinder(TEXT("/Game/Input/IMC_ChracterInput"));
    if (MappingContextFinder.Succeeded())
    {
        DefaultMappingContext = MappingContextFinder.Object;
    }
    static ConstructorHelpers::FObjectFinder<UInputAction> ClickActionFinder(TEXT("/Game/Input/IA/IA_Move"));
    if (ClickActionFinder.Succeeded())
    {
        ClickAction = ClickActionFinder.Object;
    }
    static ConstructorHelpers::FObjectFinder<UInputAction> RotateCameraActionFinder(TEXT("/Game/Input/IA/IA_CameraRotate"));
    if (RotateCameraActionFinder.Succeeded())
    {
        RotateCameraAction = RotateCameraActionFinder.Object;
    }
    static ConstructorHelpers::FObjectFinder<UInputAction> MoveCameraActionFinder(TEXT("/Game/Input/IA/IA_CameraMove"));
    if (MoveCameraActionFinder.Succeeded())
    {
        MoveCameraAction = MoveCameraActionFinder.Object;
    }
    static ConstructorHelpers::FObjectFinder<UInputAction> ResetCameraActionFinder(TEXT("/Game/Input/IA/IA_ResetCamera"));
    if (ResetCameraActionFinder.Succeeded())
    {
        ResetCameraAction = ResetCameraActionFinder.Object;
    }
    static ConstructorHelpers::FObjectFinder<UInputAction> CameraZoomActionFinder(TEXT("/Game/Input/IA/IA_CameraZoom"));
    if (CameraZoomActionFinder.Succeeded())
    {
        CameraZoomAction = CameraZoomActionFinder.Object;
    }
    static ConstructorHelpers::FClassFinder<AActor> IndicatorBP(TEXT("/Game/Actor/BP_MovePoint"));
    if (IndicatorBP.Succeeded())
    {
        TargetIndicatorClass = IndicatorBP.Class;
    }
    static ConstructorHelpers::FClassFinder<AActor> SkillRangeBP(TEXT("/Game/Actor/BP_SkillRange"));
    if (SkillRangeBP.Succeeded())
    {
        SkillRangeClass = SkillRangeBP.Class;
    }
    static ConstructorHelpers::FClassFinder<AActor> AttackRangeBP(TEXT("/Game/Actor/BP_SkillIndicator"));
    if (AttackRangeBP.Succeeded())
    {
        AttackRangeClass = AttackRangeBP.Class;
    }
    static ConstructorHelpers::FClassFinder<USkillWidget> SkillWidgetBP(TEXT("/Game/Widget/W_SkillWidget"));
    if (SkillWidgetBP.Succeeded())
    {
        skillWidgetClass = SkillWidgetBP.Class;
    }
}