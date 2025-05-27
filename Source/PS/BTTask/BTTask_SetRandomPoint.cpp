// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask/BTTask_SetRandomPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_SetRandomPoint::UBTTask_SetRandomPoint()
{
	NodeName = "Set RandomPoint";
}

EBTNodeResult::Type UBTTask_SetRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    APawn* AIPawn = AICon ? AICon->GetPawn() : nullptr;
    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

    if (!AICon || !AIPawn || !BB) return EBTNodeResult::Failed;

    FVector HomeLoc = BB->GetValueAsVector(TEXT("HomeLocation"));

    FNavLocation RandomLocation;
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(AIPawn->GetWorld());
    if (NavSystem && NavSystem->GetRandomReachablePointInRadius(HomeLoc, 600.0f, RandomLocation))
    {
        BB->SetValueAsVector(TEXT("MovePoint"), RandomLocation.Location);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}
