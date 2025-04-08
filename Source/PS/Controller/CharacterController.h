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
class USkillWidget;
class ASkillRange;
class ASkillIndicator;
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

	void InitSkillMode(int32 accuracy, int32 critical, int32 damage, int32 apUsage, bool isMag, float skillRange, float attackRange);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USkillWidget> skillWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	USkillWidget* skillWidgetInstance;

	void OnRightClick();

	void StopSkillMode();

	void MoveToMouseCursor();

	void UpdateMouseCursorLocation();

	void UpdateSkillIndicatorLocation();

	void ResetCamera();

	void SetBPs();

private:
	UPROPERTY()
	TSubclassOf<AActor> TargetIndicatorClass;
	AActor* targetIndicator;

	UPROPERTY()
	TSubclassOf<AActor> SkillRangeClass;
	ASkillRange* skillRangeIndicator;

	UPROPERTY()
	TSubclassOf<AActor> AttackRangeClass;
	ASkillIndicator* attackRangeIndicator;

	ACharacterBase* playerCharacter;

	ANormalGameMode* gameMode;

	FVector stopPoint;

	bool bIsSkillMode = false;

	int32 savedAp = -1;
	float savedSkillRange = -1;

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
	float MinZoomDistance = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxZoomDistance = 2000.0f;

};
