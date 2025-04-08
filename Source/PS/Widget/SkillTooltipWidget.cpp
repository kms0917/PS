// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SkillTooltipWidget.h"
#include"Objects/SkillBase.h"

#include "Components/TextBlock.h"

void USkillTooltipWidget::SettingTooltip(USkillBase* skill)
{
	skillName->SetText(skill->skillName);
	skillScript->SetText(skill->skillScript);
	apUsage->SetText(FText::AsNumber(skill->apUsage));
}
