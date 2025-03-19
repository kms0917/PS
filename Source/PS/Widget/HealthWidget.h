// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UProgressBar* healthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* healthText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* turnText;

	void SetHealthBar(int currnetHp, int maxHp);
	void SetTurnText(int turn);
};
