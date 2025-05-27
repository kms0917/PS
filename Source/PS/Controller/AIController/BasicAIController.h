// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BasicAIController.generated.h"

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* PatrolBT;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* CombatBT;
};
