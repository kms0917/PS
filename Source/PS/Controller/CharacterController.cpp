// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include "Camera/CameraComponent.h"


ACharacterController::ACharacterController()
{
	bShowMouseCursor = true; // 마우스 커서 표시
	DefaultMouseCursor = EMouseCursor::Default;
    ClickAction = nullptr; 
    CameraMoveSpeed = 1000.0f;
    CameraRotateSpeed = 50.0f;

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
    static ConstructorHelpers::FClassFinder<AActor> IndicatorBP(TEXT("/Game/Actor/BP_MovePoint")); 
    if (IndicatorBP.Succeeded())
    {
        TargetIndicatorClass = IndicatorBP.Class;
    }
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
        CameraComponent = ControlledPawn->FindComponentByClass<UCameraComponent>();
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
    // 마우스 클릭한 위치의 좌표 가져오기
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
        if (NavSystem)
        {
            FNavLocation NavLocation;
            if (NavSystem->ProjectPointToNavigation(TargetLocation, NavLocation))
            {
                if (TargetIndicator)
                {
                    TargetIndicator->SetActorLocation(NavLocation.Location + FVector(0, 0, 5));
                    TargetIndicator->SetActorHiddenInGame(false); // 보이게 하기
                }
            }
        }
    }
}

void ACharacterController::RotateCamera(const FInputActionValue& Value)
{
    if (!CameraComponent) return;


    // 입력값 가져오기 (-1: Q (반시계 방향), 1: E (시계 방향))
    float RotateValue = Value.Get<float>();

    // ✅ 현재 월드 스페이스 회전값 가져오기
    FRotator NewRotation = CameraComponent->GetComponentRotation();
    NewRotation.Yaw += RotateValue * CameraRotateSpeed * GetWorld()->GetDeltaSeconds();

    // ✅ 월드 스페이스에서 회전 적용
    CameraComponent->SetWorldRotation(NewRotation);
}

void ACharacterController::MoveCamera(const FInputActionValue& Value)
{
    if (!CameraComponent) return;

    // 입력값 가져오기 (Vector2D, X: 앞뒤(W/S), Y: 좌우(A/D))
    FVector2D MovementVector = Value.Get<FVector2D>();

    // ✅ 월드 스페이스에서 이동 방향 계산
    FVector Forward = FVector::ForwardVector;  // 월드 기준 전방 (X+ 방향)
    FVector Right = FVector::RightVector;      // 월드 기준 오른쪽 (Y+ 방향)

    FVector MoveDirection = (Forward * MovementVector.X + Right * MovementVector.Y)
        * CameraMoveSpeed * GetWorld()->GetDeltaSeconds();

    // ✅ 월드 스페이스에서 이동 적용
    CameraComponent->AddWorldOffset(MoveDirection, true);
}
