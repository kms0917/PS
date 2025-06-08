// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask/BTTask_EndTurn.h"
#include "Controller/AIController/BasicAIController.h"
#include "Character/CharacterBase.h"

UBTTask_EndTurn::UBTTask_EndTurn()
{
	NodeName = TEXT("End Turn");
}

EBTNodeResult::Type UBTTask_EndTurn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABasicAIController* AICon = Cast<ABasicAIController>(OwnerComp.GetAIOwner());
	ACharacterBase* AIPawn = AICon ? Cast<ACharacterBase>(AICon->GetPawn()) : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AICon || !AIPawn || !BB) return EBTNodeResult::Failed;

	AIPawn->bMyTurn = false;
	AICon->EndTurn();

	return EBTNodeResult::Succeeded;
}
