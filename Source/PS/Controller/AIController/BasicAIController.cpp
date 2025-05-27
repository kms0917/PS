// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/BasicAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

ABasicAIController::ABasicAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void ABasicAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (PatrolBT)
	{
		UseBlackboard(PatrolBT->BlackboardAsset, BlackboardComp);
		BlackboardComp->SetValueAsBool(TEXT("IsInCombat"), false);
		BlackboardComp->SetValueAsBool(TEXT("IsMyTurn"), false);
		BlackboardComp->SetValueAsVector(TEXT("HomeLocation"), InPawn->GetActorLocation());
		BehaviorComp->StartTree(*PatrolBT);
		UE_LOG(LogTemp, Warning, TEXT("aicontroller"));
	}
}

//새로운 BT 사용하는 함수, 아마 안쓸듯?
void ABasicAIController::SwitchBehaviorTree(UBehaviorTree* NewBT)
{
	if (NewBT && BlackboardComp)
	{
		UseBlackboard(NewBT->BlackboardAsset, BlackboardComp);
		BehaviorComp->StartTree(*NewBT);
	}
}

//매개변수 받아서 patrol/combat BT 변경하는 함수, 게임모드에서 전투 시작시 호출해야할듯
void ABasicAIController::SetIsInCombat(bool bCombat)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool("IsInCombat", bCombat);
		if (bCombat && CombatBT)
		{
			SwitchBehaviorTree(CombatBT);
		}
		else if (!bCombat && PatrolBT)
		{
			SwitchBehaviorTree(PatrolBT);
		}
	}
}
