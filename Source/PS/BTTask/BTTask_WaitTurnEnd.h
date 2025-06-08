// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WaitTurnEnd.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UBTTask_WaitTurnEnd : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_WaitTurnEnd();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	bool CheckBlackboard(UBehaviorTreeComponent& OwnerComp);
};
