// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/BasicAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Character/CharacterBase.h"
#include "GameMode/NormalGameMode.h"
#include "Objects/SkillBase.h"

#include "Perception/AISenseConfig_Sight.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "GenericTeamAgentInterface.h"
#include "ActorComponent/BuffComponent.h"
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
		SightConfig->SightRadius = 900.f;
		SightConfig->LoseSightRadius = 1200.f;
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
		BlackboardComp->SetValueAsBool(TEXT("EndMyTurn"), false);
		BlackboardComp->SetValueAsVector(TEXT("HomeLocation"), InPawn->GetActorLocation());
		BehaviorComp->StartTree(*PatrolBT);
		UE_LOG(LogTemp, Warning, TEXT("aicontroller"));
	}
}

void ABasicAIController::SwitchBehaviorTree(UBehaviorTree* NewBT)
{
	if (NewBT && BlackboardComp)
	{
		UseBlackboard(NewBT->BlackboardAsset, BlackboardComp);
		BehaviorComp->StartTree(*NewBT);
	}
}

//매개변수 받아서 patrol/combat BT 변경하는 함수, 전투 시작시 호출해야할듯
void ABasicAIController::SetIsInCombat(bool bCombat)
{
	if (BlackboardComp)
	{
		if (bCombat && BlackboardComp->GetValueAsBool("IsInCombat"))
		{
			return;
		}
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

//데미지 받았을 시
void ABasicAIController::NotifyCustomDamage()
{
	UE_LOG(LogTemp, Warning, TEXT("AI 피해 감지 → 회전 탐색 시작"));

	Step = 0;

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
		0.01f // 1초 지속
	);
}

void ABasicAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawSightConeDebug();
}

//ai의 애님 노티파이에서 실행시킬 함수, 현재 블랙보드의 target에 데미지 줌
void ABasicAIController::DoAIDamage()
{
	ACharacterBase* targetCharacter = Cast<ACharacterBase>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
	USkillBase* selectedSkill = Cast<USkillBase>(BlackboardComp->GetValueAsObject(TEXT("SelectedSkill")));
	if (targetCharacter)
	{
		// 주변 범위 감지
		TArray<FOverlapResult> Overlaps;
		FCollisionShape Sphere = FCollisionShape::MakeSphere(selectedSkill->attackRange); // 예: 1000 단위 반경
		FCollisionObjectQueryParams QueryParams;
		QueryParams.AddObjectTypesToQuery(ECC_Pawn);

		GetWorld()->OverlapMultiByObjectType(
			Overlaps,
			targetCharacter->GetActorLocation(),
			FQuat::Identity,
			QueryParams,
			Sphere
		);

		for (auto& Result : Overlaps)
		{
			ACharacterBase* NearbyChar = Cast<ACharacterBase>(Result.GetActor());
			if (NearbyChar)
			{
				NearbyChar->TargettedOn(selectedSkill->calculatedAccuracy, selectedSkill->calculatedCritical, selectedSkill->calculatedDamage, selectedSkill->bIsMag, true, selectedSkill->bIsHeal);
				NearbyChar->ReflectDamage(selectedSkill->bIsHeal);
			}
		}

		BlackboardComp->SetValueAsBool(TEXT("EndMyTurn"), true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("notify : target missing"));
	}
}

void ABasicAIController::StartTurn()
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("IsMyTurn"), true);
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), nullptr);
		BlackboardComp->SetValueAsObject(TEXT("SelectedSkill"), nullptr);
	}
	if (ControlledPawn)
	{
		ControlledPawn->buffComponent->ReduceBuffCount();
	}
}

//EndTurnTask에서 실행시킬 함수, 블랙보드의 값들 초기화
void ABasicAIController::EndTurn()
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("IsMyTurn"), false);
		BlackboardComp->SetValueAsBool(TEXT("EndMyTurn"), false);
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), nullptr);
		BlackboardComp->SetValueAsObject(TEXT("SelectedSkill"), nullptr);
	}
	ANormalGameMode* GM = Cast<ANormalGameMode>(UGameplayStatics::GetGameMode(this));
	if (GM)
	{
		GM->EndTurn();
	}
}

USkillBase* ABasicAIController::GetSelectedSkill()
{
	return Cast<USkillBase>(BlackboardComp->GetValueAsObject(TEXT("SelectedSkill")));
}

ACharacterBase* ABasicAIController::GetTargetCharacter()
{
	return Cast<ACharacterBase>(BlackboardComp->GetValueAsObject(TEXT("TargetActor")));
}

void ABasicAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && !(BlackboardComp->GetValueAsBool("IsInCombat")))
	{
		// 감지 시작
		UE_LOG(LogTemp, Warning, TEXT("감지 시작: %s"), *Actor->GetName());
		CurrentlySeenTarget = Actor;

		// 타이머 시작 (이미 있으면 무시)
		if (!GetWorld()->GetTimerManager().IsTimerActive(ConfirmSightingTimerHandle))
		{
			GetWorld()->GetTimerManager().SetTimer(
				ConfirmSightingTimerHandle,
				this,
				&ABasicAIController::ConfirmSighting,
				ConfirmSightingTime,
				false
			);
		}
	}
	else
	{
		// 감지 종료 → 타이머 취소
		UE_LOG(LogTemp, Warning, TEXT("감지 종료: %s"), *Actor->GetName());

		GetWorld()->GetTimerManager().ClearTimer(ConfirmSightingTimerHandle);
		CurrentlySeenTarget = nullptr;
	}
}

//데미지 받았을 시 주변 탐색
void ABasicAIController::PerformScanRotation()
{
	if (!ControlledPawn || BlackboardComp->GetValueAsBool("IsInCombat")) return;

	if (Step >= MaxSteps)
	{
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("회전 탐색 종료"));
		Step = 0;
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

//아군 캐릭터 감지 확인 됐을 시 전투 시작
void ABasicAIController::ConfirmSighting()
{
	if (CurrentlySeenTarget.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("전투 시작 조건 만족: %s"), *CurrentlySeenTarget->GetName());
		CurrentlySeenTarget = nullptr;
		ANormalGameMode* gameMode = Cast<ANormalGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gameMode)
		{
			gameMode->StartCombat(GetPawn()->GetActorLocation());
		}
	}
}
