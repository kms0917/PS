// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckMyTurn.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PS_API UBTDecorator_CheckMyTurn : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckMyTurn();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
