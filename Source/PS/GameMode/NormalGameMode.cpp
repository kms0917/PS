// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"

#include "Character/CharacterBase.h"
#include "Controller/CharacterController.h"

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
}
