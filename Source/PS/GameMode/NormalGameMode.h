// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NormalGameMode.generated.h"

class ACharacterBase;
class ACharacterController;
/**
 * 
 */
UCLASS()
class PS_API ANormalGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ANormalGameMode();

protected:
	virtual void BeginPlay() override;

public:
	bool bIsBattle;

	TArray<ACharacterBase*> battleCharacters;		//전투 시작시 여기에 모든 캐릭터들 다 넣은 후 턴순서 계산해 정렬한 후 차례대로 관리
	TArray<ACharacterBase*> freindlyCharacters;
	ACharacterBase* currentCharacter;		//현재 빙의중인 캐릭터, 전투시엔 적 캐릭터가 들어올 수 도 있음

	ACharacterController* playerController;		//빙의중인 캐릭터 바꾸기 위해 필요
};
