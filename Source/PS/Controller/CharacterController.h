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
class USkillInfoWidget;
class ASkillRange;
class ASkillIndicator;
class AMovePoint;
class UMultiTargetSkillWidget;

/**
 * 
 */
UCLASS()
class PS_API ACharacterController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACharacterController();

	void InitTurn();

	UFUNCTION()
	void EndTurn();

	void EndCombat();

	void InitSkillMode(int32 accuracy, int32 critical, int32 damage, int32 apUsage, bool isMag, float skillRange, float attackRange, bool isHeal, bool isTargeting, int multiTargetingNum);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float totalDistance;

	FRotator cameraRotation;

	bool bIsStop = true;
	bool bIsMoving = false;
	bool bUseSkill = false;

	ACharacterBase* playerCharacter;

	UPROPERTY(EditAnywhere)
	TArray<ACharacterBase*> targettedCharacter;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	USkillWidget* skillWidgetInstance;
	
	void InitAttack();

	void StartCombatMode();
	void ShowCombatStartWidget();

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
	UPROPERTY()
	UInputAction* AttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<USkillWidget> skillWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMultiTargetSkillWidget> multiTargetSkillWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UMultiTargetSkillWidget* multiTargetSkillWidgetInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> battleStartWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UUserWidget* battleStartWidget;

	void OnRightClick();
	void OnLeftClick();
	void StopSkillMode();
	void EndSkillMode();
	void MoveTotargetIndicator();
	void UpdateMouseCursorLocation();
	void UpdateSkillIndicatorLocation();
	void ResetCamera();
	void SetBPs();
	bool IsMouseOverUI() const;

private:
	UPROPERTY()
	TSubclassOf<AActor> TargetIndicatorClass;
	AMovePoint* targetIndicator;

	UPROPERTY()
	TSubclassOf<AActor> SkillRangeClass;
	ASkillRange* skillRangeIndicator;

	UPROPERTY()
	TSubclassOf<AActor> AttackRangeClass;
	ASkillIndicator* attackRangeIndicator;

	ANormalGameMode* gameMode;

	UPROPERTY(EditAnyWhere)
	FVector stopPoint;

	bool bIsShortDistanceMove = false;
	FVector shortMoveTarget = FVector::ZeroVector;

	bool bIsSkillMode = false;

	int32 savedAp = -1;
	float savedSkillRange = -1;
	bool bIsTargeting = false;
	bool bCanMoveCamera = true;
	int multiTargettingNum = 0;

	FVector2D LastMousePosition = FVector2D::ZeroVector;
	FVector attackPoint = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	void RotateCamera(const FInputActionValue& Value);
	void MoveCamera(const FInputActionValue& Value);
	void ZoomCamera(const FInputActionValue& Value);
	void UpdateCameraRotation();
	void CheckCameraAttachtoCharacter();
	void CheckCharacterMove();
	void CheckShortMove();
	void SetNavPath();

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraMoveSpeed;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraRotateSpeed;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxDistance = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ZoomSpeed = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinZoomDistance = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxZoomDistance = 3000.0f;

	UPROPERTY()
	float CombatStartDisplayTime = 1.0f;

	FTimerHandle CombatStartTimerHandle;

	void HideCombatStartWidget();
};
