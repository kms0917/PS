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
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SWindow.h"
#include "Widgets/SViewport.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"

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

//턴이 왔을때 실행시킬 함수, 행동력 회복 등의 로직, 위젯 내용 갱신도 해야함
void ACharacterController::OnTurnChanged() 
{
    playerCharacter = Cast<ACharacterBase>(GetPawn());      //빙의 캐릭터가 바뀐 후 호출되어야 함, 게임모드에서 관리, 적 캐릭터면 기능 다 잠궈야 함
    
}

//위젯에서 스킬 클릭 시 스킬모드 진입
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
            targetIndicator->SetActorHiddenInGame(false);
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
        EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ACharacterController::OnLeftClick);
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
    CheckShortMove();
}

//우클릭 해 이동
void ACharacterController::OnRightClick()
{
    if (IsMouseOverUI() || playerCharacter->IsMontagePlayed() || (playerCharacter->bIsBattle && !playerCharacter->bMyTurn))
    {
        return;
    }
    if (bIsSkillMode)
    {
        StopSkillMode();
    }
    else
    {
        MoveTotargetIndicator();
    }
}

//좌클릭 해 공격
void ACharacterController::OnLeftClick()
{
    if (!bIsSkillMode || (playerCharacter->bIsBattle && !playerCharacter->bMyTurn)) return;

    if (attackRangeIndicator && targetIndicator && stopPoint == FVector::ZeroVector)
    {
        //attackRangeIndicator->InitAttack();     //skillInstance에 데미지 받을 캐릭터들 세팅
        attackPoint = attackRangeIndicator->GetActorLocation();
        if (playerCharacter->bIsBattle)
        {
            playerCharacter->currentAp -= savedAp;
            skillWidgetInstance->UpdateButtons(playerCharacter->currentAp);
        }
        MoveTotargetIndicator();
        EndSkillMode();
    }
}

//애님 노티파이에서 공격 할 때 사용
void ACharacterController::InitAttack()
{
    if (attackRangeIndicator)
    {
        attackRangeIndicator->InitAttack();
        attackRangeIndicator->Destroy();
    }
}

//전투 시작시 이동 및 입력 멈춤
void ACharacterController::StartCombatMode()
{   
    if (bUseSkill)
    {
        bUseSkill = false;
        StopSkillMode();
    }
    StopMovement();
    skillWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
}

//스킬 취소로 스킬모드 종료
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
    playerCharacter->currentUsedSkill = nullptr;
}

//스킬 사용으로 스킬모드 종료
void ACharacterController::EndSkillMode()
{
    bIsSkillMode = false;
    savedAp = -1;
    savedSkillRange = -1;
    if (skillRangeIndicator)
    {
        skillRangeIndicator->Destroy();
    }
    //if (attackRangeIndicator)
    //{
    //    attackRangeIndicator->SetActorHiddenInGame(true);
    //}
    bUseSkill = true;
}

//마우스 우클릭 시 이동
void ACharacterController::MoveTotargetIndicator()
{
    if (bIsStop && !(targetIndicator->IsHidden()))
    {
        if (playerCharacter->bIsBattle && stopPoint != FVector::ZeroVector && !bIsSkillMode)  // 배틀 모드일 때 이동 거리 제한 적용
        {
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, stopPoint);
            playerCharacter->currentMoveSpeed = 0;

        }
        else if (stopPoint == FVector::ZeroVector) // 일반 모드에서는 마우스 클릭 위치로 바로 이동
        {
            if (totalDistance <= 0.50)
            {
                shortMoveTarget = targetIndicator->GetActorLocation();
                bIsShortDistanceMove = true;
            }
            else
            {
                UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, targetIndicator->GetActorLocation());
            }
            if (playerCharacter->bIsBattle)
            {
                playerCharacter->currentMoveSpeed -= totalDistance;
            }
        }
    }
}

//마우스 위치에 targetIndicator 표시
void ACharacterController::UpdateMouseCursorLocation()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

    if (HitResult.bBlockingHit)
    {
        FVector TargetLocation = HitResult.ImpactPoint;
        targetIndicator->SetActorLocation(TargetLocation);
        targetIndicator->SetActorHiddenInGame(false);
    }
}

//스킬모드에서 자동이동점 계산 및 표시
void ACharacterController::UpdateSkillIndicatorLocation()
{
    if (!attackRangeIndicator->WasRecentlyRendered(0.0f)|| !targetIndicator || !playerCharacter) return;

    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
    if (!HitResult.bBlockingHit) return;

    AActor* HitActor = HitResult.GetActor();
    if (HitActor)
    {
        // 특정 클래스인지 확인
        if (HitActor->IsA(ASkillRange::StaticClass()))
        {
            targetIndicator->SetActorHiddenInGame(true);
            bIsMoving = true;
        }
    }

    FVector MouseLocation = HitResult.ImpactPoint;
    MouseLocation.Z += 5.0f;
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

    if (bFound && !bIsMoving)
    {
        targetIndicator->SetActorLocation(BestLocation.Location);
        targetIndicator->SetActorHiddenInGame(false);
    }
    else
    {
        targetIndicator->SetActorHiddenInGame(true);
    }
}

//카메라 캐릭터 위치로 이동
void ACharacterController::ResetCamera()
{
    if (!SpringArmComponent) return;

    FVector CharacterLocation = playerCharacter->GetActorLocation();
    SpringArmComponent->SetWorldLocation(CharacterLocation);
}

//카메라 회전
void ACharacterController::RotateCamera(const FInputActionValue& Value)
{
    if (!SpringArmComponent) return;

    float RotateValue = Value.Get<float>();

    FRotator NewRotation = SpringArmComponent->GetComponentRotation();

    NewRotation.Yaw += RotateValue * CameraRotateSpeed * GetWorld()->GetDeltaSeconds();

    SpringArmComponent->SetWorldRotation(NewRotation);
}

//카메라 이동
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

//카메라 줌
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

//위젯 컴포넌트의 각도조절 위해 카메라 각도 저장
void ACharacterController::UpdateCameraRotation()
{
    cameraRotation = SpringArmComponent->GetComponentRotation();
}

//카메라가 캐릭터와 붙어있는지 확인
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

//현재 캐릭터가 움직이고 있는지 확인 및 자동이동 후 스킬 사용까지
void ACharacterController::CheckCharacterMove()
{
    if (playerCharacter && playerCharacter->GetVelocity().SizeSquared() <= 0.0f)
    {
        bIsStop = true;
        if (bUseSkill && bIsMoving)
        {
            playerCharacter->OnSkillAutoMoveFinished(attackPoint);
            bUseSkill = false;
        }
        bIsMoving = false;
    }
    else
    {
        targetIndicator->SetActorHiddenInGame(true);
        bIsStop = false;
        bIsMoving = true;
    }
}

//SimpleMove로 처리되지 않는 매우 작은 움직임 처리
void ACharacterController::CheckShortMove()
{
    if (bIsShortDistanceMove && shortMoveTarget != FVector::ZeroVector)
    {
        FVector Delta = shortMoveTarget - playerCharacter->GetActorLocation();
        Delta.Z = 0.f;

        if (!Delta.IsNearlyZero(5.0f))
        {
            FVector Direction = Delta.GetSafeNormal();
            playerCharacter->AddMovementInput(Direction, 1.0f);
        }
        else
        {
            shortMoveTarget = FVector::ZeroVector;
            bIsShortDistanceMove = false; // 도착 완료
        }
    }
}

//targetIndicator까지의 네비메시 경로 표시
void ACharacterController::SetNavPath()
{
    if (playerCharacter->bIsBattle && !playerCharacter->bMyTurn) return;
    
    if (targetIndicator && bIsStop && !bIsMoving)
    {
        FVector CharacterLocation = playerCharacter->GetActorLocation();
        UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
        FNavLocation NavLocation;
        if (NavSystem->ProjectPointToNavigation(targetIndicator->GetActorLocation(), NavLocation))
        {
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

                    if (playerCharacter->bIsBattle) // 전투 모드일 때만 제한 적용
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
            }
        }
    }
}

//BP들 세팅
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
    static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionFinder(TEXT("/Game/Input/IA/IA_Attack"));
    if (AttackActionFinder.Succeeded())
    {
        AttackAction = AttackActionFinder.Object;
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

//마우스가 위젯에 올라가있는지 체크해 위젯에 올라갈 시 입력 안되도록
bool ACharacterController::IsMouseOverUI() const
{
    FWidgetPath widgetPath = FSlateApplication::Get().LocateWindowUnderMouse(FSlateApplication::Get().GetCursorPos(), FSlateApplication::Get().GetInteractiveTopLevelWindows(), true);
    TSharedPtr viewPort = FSlateApplication::Get().GetGameViewport();
    bool overViewPort = widgetPath.IsValid() && (widgetPath.GetLastWidget() == viewPort.ToSharedRef());

    return !overViewPort;
}
