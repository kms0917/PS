// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DistanceWidget.h"
#include "Controller/CharacterController.h"
#include "GameMode/NormalGameMode.h"
#include "Character/CharacterBase.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UDistanceWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (playerController && DistanceText && ReachableText)
	{
		if (playerController->playerCharacter->bIsBattle && (!playerController->playerCharacter->bMyTurn || playerController->bIsMoving) || playerController->bUseSkill || playerController->playerCharacter->IsMontagePlayed())
		{
			DistanceText->SetVisibility(ESlateVisibility::Collapsed);
			ReachableText->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
		DistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.2f m"), playerController->totalDistance)));
		//if (!playerController->bIsStop)
		//{
		//	DistanceText->SetVisibility(ESlateVisibility::Collapsed);
		//}
		if ((playerController->playerCharacter->bIsBattle && playerController->totalDistance > playerController->playerCharacter->currentMoveSpeed))
		{
			DistanceText->SetVisibility(ESlateVisibility::Collapsed);
			ReachableText->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			DistanceText->SetVisibility(ESlateVisibility::Visible);
			ReachableText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UDistanceWidget::NativeConstruct()
{
	Super::NativeConstruct();

	playerController = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	gameMode = Cast<ANormalGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (ReachableText)
	{
		ReachableText->SetText(FText::FromString(TEXT("도달 불가")));
		ReachableText->SetVisibility(ESlateVisibility::Collapsed);	//일단 끄기, 키는게 좋을지는 나중에 결정
	}
}