// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetAttackTarget_Offensive.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UBTTask_SetAttackTarget_Offensive : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetAttackTarget_Offensive();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	bool FindAttackPointAlongPath(const FVector& StartLocation, class ACharacterBase* Target, float SkillRange, float MoveRange, FVector& OutBestPoint, AActor* AIActor);
	
};
