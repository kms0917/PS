// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SkillButtonWidget.h"
#include "Character/CharacterBase.h"
#include "ActorComponent/SkillComponent.h"
#include "Objects/SkillBase.h"
#include "Widget/SkillTooltipWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USkillButtonWidget::SettingButton(USkillBase* Skill, int32 index)
{
	skillIndex = index;
	skillAp = Skill->apUsage;
	if (skillButton)	//스킬 정보를 이용해 버튼 정보 구성
	{
		skillNameText->SetText(Skill->skillName);

		if (TooltipWidgetClass)
		{
			USkillTooltipWidget* tooltipWidget = CreateWidget<USkillTooltipWidget>(this, TooltipWidgetClass); 
			if (tooltipWidget)
			{
				tooltipWidget->SettingTooltip(Skill);
				skillButton->SetToolTip(tooltipWidget);
			}
		}
	}
}

void USkillButtonWidget::NativeDestruct()	//델리게이트 및 바인딩 해제
{
	if (skillButton && skillButton->OnClicked.IsBound())
	{
		skillButton->OnClicked.Clear();
		if (skillButton->GetToolTip())
		{
			//skillButton->GetToolTip()->RemoveFromParent();
		}
	}
	Super::NativeDestruct();
}

void USkillButtonWidget::HandleButtonClicked()		//이 함수로 바인딩 후 이 함수에서 델리게이트 호출
{
	OnSkillButtonClicked.ExecuteIfBound(skillIndex);
}

void USkillButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (skillButton)
	{
		skillButton->OnClicked.AddDynamic(this, &USkillButtonWidget::HandleButtonClicked);
	}
}
