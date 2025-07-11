// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MultiTargetSkillWidget.h"

void UMultiTargetSkillWidget::SetTargetCount(int32 CurrentTargets)
{
	if (TargetTextBlock)
	{
		FText TargetCountText = FText::Format(NSLOCTEXT("MultiTargetSkillWidget", "TargetCountFormat", "대상선택 {0} / {1}"), FText::AsNumber(CurrentTargets), FText::AsNumber(TargetingNum));
		TargetTextBlock->SetText(TargetCountText);
	}
}

void UMultiTargetSkillWidget::SetMultiTargetWidget(int32 TargetNum)
{
	this->TargetingNum = TargetNum;
}