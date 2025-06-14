// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask/BTTask_WaitTurnEnd.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_WaitTurnEnd::UBTTask_WaitTurnEnd()
{
	NodeName = TEXT("Wait Turn End");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_WaitTurnEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CheckBlackboard(OwnerComp))
	{
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::InProgress;
}

void UBTTask_WaitTurnEnd::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (CheckBlackboard(OwnerComp))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

bool UBTTask_WaitTurnEnd::CheckBlackboard(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return false;

	return BlackboardComp->GetValueAsBool(TEXT("EndMyTurn"));
}
