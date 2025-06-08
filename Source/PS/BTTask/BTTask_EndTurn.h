// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EndTurn.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UBTTask_EndTurn : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_EndTurn();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
