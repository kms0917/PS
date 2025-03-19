// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "CharacterController.generated.h"

class USpringArmComponent;
class ACharacterBase;
class ANormalGameMode;
/**
 * 
 */
UCLASS()
class PS_API ACharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACharacterController();

	void OnTurnChanged();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float totalDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsReachable;

	FRotator cameraRotation;

	bool bIsStop = true;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY()
	UInputAction* ClickAction;

	UPROPERTY()
	UInputAction* RotateCameraAction;

	UPROPERTY()
	UInputAction* MoveCameraAction;

	UPROPERTY()
	UInputAction* ResetCameraAction;

	UPROPERTY()
	UInputAction* CameraZoomAction;

	void OnRightClick();

	void MoveToMouseCursor();

	void UpdateMouseCursorLocation();

	void ResetCamera();

	void SetBPs();

private:
	UPROPERTY()
	TSubclassOf<AActor> TargetIndicatorClass;

	AActor* targetIndicator;

	ACharacterBase* playerCharacter;

	ANormalGameMode* gameMode;

	FVector stopPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	void RotateCamera(const FInputActionValue& Value);
	void MoveCamera(const FInputActionValue& Value);
	void ZoomCamera(const FInputActionValue& Value);
	void UpdateCameraRotation();

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotateSpeed;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinZoomDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxZoomDistance = 3000.0f;

};
