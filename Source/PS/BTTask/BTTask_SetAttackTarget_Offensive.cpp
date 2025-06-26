// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask/BTTask_SetAttackTarget_Offensive.h"
#include "Objects/SkillBase.h"
#include "Character/CharacterBase.h"
#include "Controller/AIController/BasicAIController.h"
#include "ActorComponent/SkillComponent.h"
#include "GameMode/NormalGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

UBTTask_SetAttackTarget_Offensive::UBTTask_SetAttackTarget_Offensive()
{
	NodeName = "Set Attack Target Offensive";
}

EBTNodeResult::Type UBTTask_SetAttackTarget_Offensive::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABasicAIController* AICon = Cast<ABasicAIController>(OwnerComp.GetAIOwner());
	ACharacterBase* AIPawn = AICon ? Cast<ACharacterBase>(AICon->GetPawn()) : nullptr;
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AICon || !AIPawn || !BB) return EBTNodeResult::Failed;

	ANormalGameMode* gameMode = Cast<ANormalGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	TArray<ACharacterBase*> characters = gameMode->friendlyCharacters;
	TArray<USkillBase*> skills = AIPawn->skillComponent->skillList;

	ACharacterBase* LowestHPCharacter = nullptr;
	USkillBase* SkillToUse = nullptr;
	float LowestHP = MAX_FLT;
	FVector BestMovePoint = FVector::ZeroVector;
	float moveSpeed = AIPawn->currentMoveSpeed;

	for (ACharacterBase* character : characters)
	{
		for (USkillBase* skill : skills)
		{
			float skillRange = skill->skillRange;
			FVector testPoint;
			
			if (FindAttackPointAlongPath(AIPawn->GetActorLocation(), character, skillRange, moveSpeed, testPoint, AICon->GetPawn()))
			{
				float HP = character->currentHp; // 혹은 GetHealth() 등
				if (HP < LowestHP)
				{
					LowestHP = HP;
					LowestHPCharacter = character;
					SkillToUse = skill;
					BestMovePoint = testPoint;
				}
			}
		}
	}
	
	if (LowestHPCharacter && SkillToUse)
	{
		BB->SetValueAsObject("TargetActor", LowestHPCharacter);
		BB->SetValueAsVector("MovePoint", BestMovePoint);
		BB->SetValueAsObject("SelectedSkill", SkillToUse); // 필요시
		return EBTNodeResult::Succeeded;
	}
	else
	{
		// 가장 가까운 캐릭터로 이동
		ACharacterBase* Closest = nullptr;
		float ClosestDist = MAX_FLT;

		for (ACharacterBase* character : characters)
		{
			float dist = FVector::Dist(AIPawn->GetActorLocation(), character->GetActorLocation());
			if (dist < ClosestDist)
			{
				ClosestDist = dist;
				Closest = character;
			}
		}

		if (Closest)
		{
			FVector StartLocation = AIPawn->GetActorLocation();
			FVector EndLocation = Closest->GetActorLocation();

			UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			if (!NavSys) return EBTNodeResult::Failed;

			UNavigationPath* NavPath = NavSys->FindPathToActorSynchronously(GetWorld(), StartLocation, Closest);
			if (!NavPath || NavPath->PathPoints.Num() < 2) return EBTNodeResult::Failed;

			float maxMoveDistance = moveSpeed * 100.f;
			float totalPathDistance = 0.f;
			bool reachedLimit = false;

			FVector previousEnd = NavPath->PathPoints[0];
			FVector stopPoint = FVector::ZeroVector;

			for (int i = 1; i < NavPath->PathPoints.Num(); i++)
			{
				FVector Start = previousEnd;
				FVector End = NavPath->PathPoints[i];
				float SegmentLength = FVector::Dist(Start, End);

				if (!reachedLimit && totalPathDistance + SegmentLength >= maxMoveDistance)
				{
					// moveSpeed 도달 전에 넘치는 경우 → 보간해서 중간 지점 계산
					float remainingDistance = maxMoveDistance - totalPathDistance;
					FVector Direction = (End - Start).GetSafeNormal();
					stopPoint = Start + Direction * remainingDistance;
					reachedLimit = true;

					// 해당 위치를 네비 메시 위로 보정
					FNavLocation NavLoc;
					if (NavSys->ProjectPointToNavigation(stopPoint, NavLoc))
					{
						BB->SetValueAsVector("MovePoint", NavLoc.Location);
						BB->ClearValue("TargetActor");
						BB->ClearValue("SelectedSkill");
						return EBTNodeResult::Succeeded;
					}
					else
					{
						return EBTNodeResult::Failed;
					}
				}

				totalPathDistance += SegmentLength;
				previousEnd = End;
			}
		}
	}

	return EBTNodeResult::Failed;
}

//가장 체력이 작은 아군 캐릭터에게 공격
bool UBTTask_SetAttackTarget_Offensive::FindAttackPointAlongPath(const FVector& StartLocation, ACharacterBase* Target, float SkillRange, float MoveRange, FVector& OutBestPoint, AActor* AIActor)
{
	if (!Target) return false;

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Target);
	if (!NavSystem) return false;

	const int NumSamples = 100;
	const float Offset = 2.f / NumSamples;
	const float Increment = PI * (3.f - FMath::Sqrt(5.f));

	FVector TargetLocation = Target->GetActorLocation();
	TArray<FVector> Samples;

	// Fibonacci Sphere 기반 샘플링 (바닥 기준)
	for (int i = 0; i < NumSamples; ++i)
	{
		float Y = ((i * Offset) - 1.f) + (Offset / 2.f);
		float Radius = FMath::Sqrt(1 - Y * Y);
		float Theta = i * Increment;

		float X = FMath::Cos(Theta) * Radius;
		float Z = FMath::Sin(Theta) * Radius;

		// 공격 대상의 위치를 중심으로 SkillRange 반지름 내 샘플
		FVector Sample = FVector(X, Y, Z) * SkillRange + TargetLocation;
		Samples.Add(Sample);
	}

	float ShortestPath = TNumericLimits<float>::Max();
	FNavLocation BestLocation;
	bool bFound = false;
	
	for (const FVector& Sample : Samples)
	{
		FNavLocation Projected;
		if (!NavSystem->ProjectPointToNavigation(Sample, Projected)) continue;

		// 장애물 검사 (샘플 → Target)
		FHitResult ObstacleHit;
		FCollisionQueryParams Params(FName("SkillTrace"), true, AIActor);
		Params.AddIgnoredActor(Target);
		Params.AddIgnoredActor(AIActor);

		FVector TraceStart = Projected.Location + FVector(0, 0, 5);
		FVector TraceEnd = TargetLocation + FVector(0, 0, 5);

		bool bBlocked = Target->GetWorld()->LineTraceSingleByChannel(
			ObstacleHit, TraceStart, TraceEnd, ECC_Visibility, Params
		);

		if (bBlocked) continue;

		// AI → 샘플 위치까지 네비 경로 계산
		UNavigationPath* Path = NavSystem->FindPathToLocationSynchronously(
			Target->GetWorld(), StartLocation, Projected.Location
		);

		if (!Path || !Path->IsValid() || Path->PathPoints.Num() < 2) continue;

		// 경로 거리 측정
		float PathLength = 0.f;
		for (int i = 1; i < Path->PathPoints.Num(); ++i)
		{
			PathLength += FVector::Dist(Path->PathPoints[i - 1], Path->PathPoints[i]);
		}
		
		if (PathLength < ShortestPath && PathLength <= MoveRange * 100.f)
		{
			ShortestPath = PathLength;
			BestLocation = Projected;
			bFound = true;
		}
	}
	
	if (bFound)
	{
		OutBestPoint = BestLocation.Location;
		return true;
	}

	return false;
}