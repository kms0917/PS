// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTooltipWidget.generated.h"

class USkillBase;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PS_API USkillTooltipWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SettingTooltip(USkillBase* skill);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* skillName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* skillScript;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* apUsage;

};
