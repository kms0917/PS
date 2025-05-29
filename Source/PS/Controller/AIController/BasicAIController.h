// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "BasicAIController.generated.h"

class ACharacterBase;
/**
 * 
 */
UCLASS()
class PS_API ABasicAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABasicAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	void SwitchBehaviorTree(UBehaviorTree* NewBT);
	void SetIsInCombat(bool bCombat);

	// 데미지를 받았을 때 호출 (외부에서 사용)
	void NotifyCustomDamage();
	void DrawSightConeDebug();
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* PatrolBT;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* CombatBT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void PerformScanRotation();

	FTimerHandle RotationTimerHandle;

	int32 Step = 1;
	const int32 MaxSteps = 4;            // 딱 4번 회전
	const float AnglePerStep = 90.f;     // 90도씩
	const float RotationInterval = 1.5f; // 회전 간격

	ACharacterBase* ControlledPawn;
};
