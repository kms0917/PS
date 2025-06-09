// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"

#include "Character/CharacterBase.h"
#include "Controller/CharacterController.h"
#include "Controller/AIController/BasicAIController.h"

ANormalGameMode::ANormalGameMode()
{
	static ConstructorHelpers::FClassFinder<ACharacterBase> PlayerPawnClass(TEXT("/Game/Characters/test"));
    if (PlayerPawnClass.Succeeded())
    {
        DefaultPawnClass = PlayerPawnClass.Class;
    }
    PlayerControllerClass = ACharacterController::StaticClass();
}

void ANormalGameMode::BeginPlay()
{
    Super::BeginPlay();

    playerController = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController());
    currentCharacter = Cast<ACharacterBase>(playerController->GetPawn());
	if (currentCharacter)
	{
		freindlyCharacters.Add(currentCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("currnetCharacter Cast Failed"));
	}
}

//정렬된 battleCharacters에서 가장 앞에 있는 캐릭터의 컨트롤러를 ai컨트롤러로 캐스팅, 성공 시 ai 컨트롤러의 턴 개시 함수 실행, 실패 시 해당 캐릭터로 플레이어컨트롤러 빙의
void ANormalGameMode::InitTurn()
{
	if (battleCharacters[0]->GetGenericTeamId() == FGenericTeamId(0))	//아군일경우
	{
		currentCharacter = battleCharacters[0];
		playerController->Possess(battleCharacters[0]);
		playerController->InitTurn();
	}
	else
	{
		ABasicAIController* AICon = Cast<ABasicAIController>(battleCharacters[0]->GetController());
		if (AICon)
		{
			AICon->StartTurn();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AICon Cast Failed_GameMode"));
		}
	}
}

void ANormalGameMode::GetEXP()
{
	for (int i = 0; i < freindlyCharacters.Num(); i++)
	{
		freindlyCharacters[i]->GetEXP();
	}
}

//전투 시작 시 전투 캐릭터들의 배열에 캐릭터 추가
void ANormalGameMode::RegisterBattleCharacters(ACharacterBase* registedCharacter)
{
    if (registedCharacter && !battleCharacters.Contains(registedCharacter))
    {
        battleCharacters.Add(registedCharacter);
		registedCharacter->bIsBattle = true;
    }
}

//전투중인 캐릭터들의 배열 비움
void ANormalGameMode::ClearBattleCharacters()
{
    battleCharacters.Empty();
}

//전투 시작위치를 받아와 해당 지점 근처의 캐릭터를 스캔해 전투 시작 및 턴 순서 계산해 전투 캐릭터 배열 정렬 후 위젯컴포넌트 반영까지
void ANormalGameMode::StartCombat(FVector BattleLocation)
{
	// 주변 범위 감지
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(7000.f); // 예: 1000 단위 반경
	FCollisionObjectQueryParams QueryParams;
	QueryParams.AddObjectTypesToQuery(ECC_Pawn);

	GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		BattleLocation,
		FQuat::Identity,
		QueryParams,
		Sphere
	);

	for (auto& Result : Overlaps)
	{
		ACharacterBase* NearbyChar = Cast<ACharacterBase>(Result.GetActor());
		if (NearbyChar)
		{
			RegisterBattleCharacters(NearbyChar);
			ABasicAIController* aiContoroller = Cast<ABasicAIController>(NearbyChar->GetController());
			if (aiContoroller)
			{
				aiContoroller->SetIsInCombat(true);
			}
		}
	}
	playerController->StartCombatMode();
	// 캐릭터별 랜덤 점수 저장용 맵
	TMap<ACharacterBase*, int> RollMap;

	for (ACharacterBase* Char : battleCharacters)
	{
		if (Char)
		{
			int Speed = Char->speed;
			int Roll = FMath::RandRange(0, Speed) + (Speed / 2);
			RollMap.Add(Char, Roll);
		}
	}

	// 저장된 점수를 기준으로 정렬
	battleCharacters.Sort([&RollMap](ACharacterBase& A, ACharacterBase& B) { return RollMap[&A] > RollMap[&B]; });

	for (int i = 0; i < battleCharacters.Num(); i++)
	{
		battleCharacters[i]->SetTurnText(i);
	}
	InitTurn();
}

//playerController의 EndTurn과 AIController의 EndTurn에서 호출, battleCharacters배열정리 후 TurnText 갱신 및 전투 종료 확인 후 InitTurn호출
void ANormalGameMode::EndTurn()
{

}
