// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SkillTooltipWidget.h"
#include"Objects/SkillBase.h"

#include "Components/TextBlock.h"

void USkillTooltipWidget::SettingTooltip(USkillBase* skill)
{
	skillName->SetText(skill->skillName);
	skillScript->SetText(skill->skillScript);
	apUsage->SetText(FText::FromString(FString::Printf(TEXT("AP : %d"), skill->apUsage)));
	accuracy->SetText(FText::FromString(FString::Printf(TEXT("Accuracy : %d"), skill->calculatedAccuracy)));
	critical->SetText(FText::FromString(FString::Printf(TEXT("Critical : %d"), skill->calculatedCritical)));
	if (skill->bIsHeal)
	{
		dmgOrheal->SetText(FText::FromString(FString::Printf(TEXT("Heal : %d"), skill->calculatedDamage)));
		accuracy->SetVisibility(ESlateVisibility::Hidden);
		critical->SetVisibility(ESlateVisibility::Hidden);
	}
	else if (skill->bIsBuff)
	{
		dmgOrheal->SetVisibility(ESlateVisibility::Hidden);
		accuracy->SetVisibility(ESlateVisibility::Hidden);
		critical->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		dmgOrheal->SetText(FText::FromString(FString::Printf(TEXT("Damage : %d"), skill->calculatedDamage)));
	}
}
