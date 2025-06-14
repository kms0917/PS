// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask/BTTask_PlayAnimMontage.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

#include "Character/CharacterBase.h"
#include "Objects/SkillBase.h"

UBTTask_PlayAnimMontage::UBTTask_PlayAnimMontage()
{
	NodeName = TEXT("Play Anim Montage");
}

EBTNodeResult::Type UBTTask_PlayAnimMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	ACharacterBase* AIPawn = AICon ? Cast<ACharacterBase>(AICon->GetPawn()) : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AICon || !AIPawn || !BB) return EBTNodeResult::Failed;

	ACharacterBase* target = Cast<ACharacterBase>(BB->GetValueAsObject(TEXT("TargetActor")));
	USkillBase* skillToUse = Cast<USkillBase>(BB->GetValueAsObject(TEXT("SelectedSkill")));
	if (target && skillToUse)
	{
		FVector Direction = target->GetActorLocation() - AIPawn->GetActorLocation();
		Direction.Z = 0.0f;
		if (!Direction.IsNearlyZero())
		{
			FRotator NewRotation = Direction.Rotation();
			AIPawn->SetActorRotation(NewRotation);
		}
		UAnimInstance* AnimInstance = AIPawn->GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(skillToUse->skillMontage, skillToUse->PlayRate);
			return EBTNodeResult::Succeeded;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("fail anim"));
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		BB->SetValueAsBool(TEXT("EndMyTurn"), true);
		UE_LOG(LogTemp, Warning, TEXT("EndMyTurn Set(애니메이션 필요 x)"));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Type();
}
