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



ACharacterController::ACharacterController()
{
	bShowMouseCursor = true; // 마우스 커서 표시
	DefaultMouseCursor = EMouseCursor::Default;
    ClickAction = nullptr; 
    CameraMoveSpeed = 1000.0f;
    CameraRotateSpeed = 50.0f;

    SetBPs();
}

void ACharacterController::BeginPlay()
{
    Super::BeginPlay();

    // Enhanced Input 시스템 적용
    if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        if (DefaultMappingContext)
        {
            InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
    if (TargetIndicatorClass)
    {
        TargetIndicator = GetWorld()->SpawnActor<AActor>(TargetIndicatorClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (TargetIndicator)
        {
            TargetIndicator->SetActorHiddenInGame(true); // 처음엔 숨김
        }
    }
    if (APawn* ControlledPawn = GetPawn())
    {
        SpringArmComponent = ControlledPawn->FindComponentByClass<USpringArmComponent>();
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
    }
}

void ACharacterController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    UpdateMouseCursorLocation();
}

void ACharacterController::OnRightClick()
{
    MoveToMouseCursor();
}

void ACharacterController::MoveToMouseCursor()
{
    ResetCamera();
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_WorldStatic, false, HitResult);

    if (HitResult.bBlockingHit)
    {
        APawn* ControlledPawn = GetPawn();
        if (ControlledPawn)
        {
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitResult.ImpactPoint);
        }
    }
}

void ACharacterController::UpdateMouseCursorLocation()
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_WorldStatic, false, HitResult);

    if (HitResult.bBlockingHit)
    {
        FVector TargetLocation = HitResult.ImpactPoint;

        UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
        if (NavSystem && GetPawn())
        {
            FNavLocation NavLocation;
            if (NavSystem->ProjectPointToNavigation(TargetLocation, NavLocation))
            {
                FVector CharacterLocation = GetPawn()->GetActorLocation();
                if (TargetIndicator)
                {
                    TargetIndicator->SetActorLocation(NavLocation.Location + FVector(0, 0, 5));
                    TargetIndicator->SetActorHiddenInGame(false); // 보이게 하기
                }

                // 네비메시 경로 계산
                UNavigationPath* NavPath = NavSystem->FindPathToLocationSynchronously(
                    this, CharacterLocation, NavLocation.Location);

                if (NavPath && NavPath->IsValid())
                {
                    // 캐릭터가 이동 중이 아니면 경로를 그리기
                    if (GetCharacter()->GetCharacterMovement()->Velocity.SizeSquared() <= 0.0f)
                    {
                        // 경로가 유효한 경우 각 PathPoint들에 대해 원통으로 연결
                        for (int32 i = 0; i < NavPath->PathPoints.Num() - 1; i++)
                        {
                            FVector Start = NavPath->PathPoints[i];
                            FVector End = NavPath->PathPoints[i + 1];

                            // 경로 사이에 원통 그리기 (흰색 원통, 반지름 10.0f, 길이 = 거리)
                            FColor CylinderColor = FColor::White;
                            float CylinderRadius = 10.0f; // 얇은 원통으로 설정
                            float CylinderLength = (End - Start).Size();

                            DrawDebugCylinder(GetWorld(), Start, End, CylinderRadius, 12, CylinderColor, false, -1, 0, 1);
                        }
                        float TotalDistance = 0.0f;
                        for (int32 i = 0; i < NavPath->PathPoints.Num() - 1; i++)
                        {
                            TotalDistance += FVector::Dist(NavPath->PathPoints[i], NavPath->PathPoints[i + 1]);
                        }
                        FString DistanceText = FString::Printf(TEXT("이동 거리: %.2f m"), TotalDistance / 100.0f);
                        GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Cyan, DistanceText);
                    }
                }
            }
        }
    }
}

void ACharacterController::ResetCamera()
{
    if (!SpringArmComponent) return;

    FVector CharacterLocation = GetPawn()->GetActorLocation();

    // ✅ 카메라의 높이는 유지 (현재 SpringArm의 Z값 사용)
    FVector NewLocation = CharacterLocation;
    NewLocation.Z = SpringArmComponent->GetComponentLocation().Z;

    // ✅ SpringArmComponent 위치 초기화
    SpringArmComponent->SetWorldLocation(NewLocation);
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
    if (!SpringArmComponent) return;

    FVector2D MovementVector = Value.Get<FVector2D>();

    FVector Forward = SpringArmComponent->GetForwardVector();  
    FVector Right = SpringArmComponent->GetRightVector();  

    Forward.Z = 0.0f;
    Right.Z = 0.0f;
    Forward.Normalize();
    Right.Normalize();

    FVector MoveDirection = (Forward * MovementVector.Y + Right * MovementVector.X)
        * CameraMoveSpeed * GetWorld()->GetDeltaSeconds();

    FVector CurrentWorldLocation = SpringArmComponent->GetComponentLocation();

    SpringArmComponent->AddWorldOffset(MoveDirection, true);

    FVector NewLocation = SpringArmComponent->GetComponentLocation();
    NewLocation.Z = CurrentWorldLocation.Z;
    SpringArmComponent->SetWorldLocation(NewLocation);
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
    static ConstructorHelpers::FClassFinder<AActor> IndicatorBP(TEXT("/Game/Actor/BP_MovePoint"));
    if (IndicatorBP.Succeeded())
    {
        TargetIndicatorClass = IndicatorBP.Class;
    }
}