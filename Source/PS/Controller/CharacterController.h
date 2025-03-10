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
/**
 * 
 */
UCLASS()
class PS_API ACharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACharacterController();

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

	void OnRightClick();

	void MoveToMouseCursor();

	void UpdateMouseCursorLocation();

	void ResetCamera();

	void SetBPs();

private:
	UPROPERTY()
	TSubclassOf<AActor> TargetIndicatorClass;

	AActor* TargetIndicator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	void RotateCamera(const FInputActionValue& Value);
	void MoveCamera(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraMoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotateSpeed;
};
