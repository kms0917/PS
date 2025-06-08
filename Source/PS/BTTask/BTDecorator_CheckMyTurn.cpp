// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask/BTDecorator_CheckMyTurn.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"

UBTDecorator_CheckMyTurn::UBTDecorator_CheckMyTurn()
{
	NodeName = TEXT("Check MyTurn");
}

bool UBTDecorator_CheckMyTurn::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlackboardComp is nullptr"));
		return false;
	}

	return BlackboardComp->GetValueAsBool(TEXT("IsMyTurn"));
}
