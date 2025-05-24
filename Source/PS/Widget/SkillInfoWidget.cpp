// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SkillInfoWidget.h"

#include "Components/TextBlock.h"

void USkillInfoWidget::SettingWidget(int32 accuracy, int32 critical, int32 damage)
{
	accuracyText->SetText(FText::AsNumber(accuracy));
	criticalText->SetText(FText::AsNumber(critical));
	damageText->SetText(FText::AsNumber(damage));
}

void USkillInfoWidget::RessetWidget()
{
	accuracyText->SetText(FText::AsNumber(0));
	criticalText->SetText(FText::AsNumber(0));
	damageText->SetText(FText::AsNumber(0));
}
