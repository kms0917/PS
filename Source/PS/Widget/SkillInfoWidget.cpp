// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SkillInfoWidget.h"

#include "Components/TextBlock.h"

void USkillInfoWidget::SettingWidget(int32 accuracy, int32 critical, int32 damage, bool isHeal)
{
	if (isHeal)
	{
		damageText->SetText(FText::FromString(FString::Printf(TEXT("Heal : %d"), damage)));
		accuracyText->SetVisibility(ESlateVisibility::Hidden);
		criticalText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		damageText->SetText(FText::FromString(FString::Printf(TEXT("Damage : %d"), damage)));
		accuracyText->SetText(FText::FromString(FString::Printf(TEXT("Accuracy : %d"), accuracy)));
		criticalText->SetText(FText::FromString(FString::Printf(TEXT("Critical : %d"), critical)));
		accuracyText->SetVisibility(ESlateVisibility::Visible);
		criticalText->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillInfoWidget::RessetWidget()
{
	accuracyText->SetText(FText::AsNumber(0));
	criticalText->SetText(FText::AsNumber(0));
	damageText->SetText(FText::AsNumber(0));
}
