// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MultiTargetSkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UMultiTargetSkillWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetTargetCount(int32 CurrentTargets);

	void SetMultiTargetWidget(int32 TargetingNum);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TargetTextBlock;

	int32 TargetingNum = 0;
};
