// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/BasicAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Character/CharacterBase.h"
#include "GameMode/NormalGameMode.h"

#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/GameplayStatics.h"

ABasicAIController::ABasicAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	// PerceptionComponent 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// 시야 설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	if (SightConfig)
	{
		SightConfig->SightRadius = 1500.f;
		SightConfig->LoseSightRadius = 1800.f;
		SightConfig->PeripheralVisionAngleDegrees = 50.f; // 전방 부채꼴 시야
		SightConfig->SetMaxAge(5.f);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = false;

		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	}

	// 감지 콜백 바인딩
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicAIController::OnTargetPerceptionUpdated);

}

void ABasicAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledPawn = Cast<ACharacterBase>(InPawn);

	if (ControlledPawn)
	{
		SetGenericTeamId(FGenericTeamId(1));
	}

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

void ABasicAIController::NotifyCustomDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("AI 피해 감지 → 회전 탐색 시작"));

	Step = 1;

	GetWorld()->GetTimerManager().SetTimer(
		RotationTimerHandle,
		this,
		&ABasicAIController::PerformScanRotation,
		RotationInterval,
		true
	);
}

void ABasicAIController::DrawSightConeDebug()
{
	if (!ControlledPawn || !SightConfig) return;

	FVector Origin = ControlledPawn->GetActorLocation();
	FRotator Facing = ControlledPawn->GetActorRotation();
	float Radius = SightConfig->SightRadius;
	float Angle = SightConfig->PeripheralVisionAngleDegrees;

	// ✅ 전방 시야 범위 디버그 (부채꼴)
	DrawDebugCone(
		GetWorld(),
		Origin,
		Facing.Vector(),       // 전방 방향
		Radius,
		FMath::DegreesToRadians(Angle),
		FMath::DegreesToRadians(Angle),
		12,
		FColor::Green,
		false,
		1.0f // 1초 지속
	);
}

void ABasicAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawSightConeDebug();
}

void ABasicAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("AI 감지 성공 → 타겟: %s"), *Actor->GetName());
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
		ANormalGameMode* gameMode = Cast<ANormalGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gameMode)
		{
			gameMode->bIsBattle = true;
		}

		// 주변 범위 감지
		TArray<FOverlapResult> Overlaps;
		FCollisionShape Sphere = FCollisionShape::MakeSphere(7000.f); // 예: 1000 단위 반경
		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_Pawn);

		GetWorld()->OverlapMultiByObjectType(
			Overlaps,
			GetPawn()->GetActorLocation(),
			FQuat::Identity,
			QueryParams,
			Sphere
		);
		for (auto& Result : Overlaps)
		{
			ACharacterBase* NearbyChar = Cast<ACharacterBase>(Result.GetActor());
			if (NearbyChar)
			{
				// GameMode에 등록
				if (gameMode)
				{
					gameMode->RegisterBattleCharacters(NearbyChar);
				}
			}
		}
	}
}

void ABasicAIController::PerformScanRotation()
{
	if (!ControlledPawn) return;

	if (Step >= MaxSteps)
	{
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("회전 탐색 종료"));
		Step = 1;
		return;
	}

	FRotator CurrentRotation = ControlledPawn->GetActorRotation();
	CurrentRotation.Yaw += AnglePerStep;
	ControlledPawn->SetActorRotation(CurrentRotation);

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->RequestStimuliListenerUpdate();
	}

	Step++;
}
