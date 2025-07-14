// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/SkillWidget.h"
#include "Character/CharacterBase.h"
#include "ActorComponent/SkillComponent.h"
#include "Objects/SkillBase.h"
#include "Widget/SkillButtonWidget.h"
#include "Controller/CharacterController.h"

#include "Components/UniformGridPanel.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void USkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EndButton)
	{
		ACharacterController* Con = Cast<ACharacterController>(GetOwningPlayer());
		if (Con)
		{
			EndButton->OnClicked.AddDynamic(Con, &ACharacterController::EndTurn);
		}
	}
}

void USkillWidget::UpdateWidget(ACharacterBase* ControlledCharacter)	//턴이 바뀔때마다 실행해야 함
{
	currentCharacter = ControlledCharacter;
	if (!currentCharacter || currentCharacter->skillComponent->skillList.Num() == 0)
	{
		GetWorld()->GetTimerManager().SetTimer(
		DelayHandle,
		[this, ControlledCharacter]()
		{
			UpdateWidget(ControlledCharacter);
		},
		0.5f,    // 1.5초 후 실행
		false     // 반복 실행
	);
		return;
	}
	SkillGridPanel->ClearChildren();
	TArray<USkillBase*> skills = currentCharacter->skillComponent->skillList;
	int Row = 0, Col = 0;
	const int MaxColumns = 5; // 한 줄에 5개씩 배치
	for (int i = 0; i < skills.Num(); i++)
	{
		USkillButtonWidget* NewButton = CreateWidget<USkillButtonWidget>(this, SkillButtonClass);
		if (NewButton)
		{
			NewButton->SettingButton(skills[i], i);
			NewButton->OnSkillButtonClicked.BindLambda([this](int32 ClickedIndex)
				{
					if (currentCharacter)
					{
						currentCharacter->UseSkill(ClickedIndex);
					}					
				});
			UUniformGridSlot* GridSlot = SkillGridPanel->AddChildToUniformGrid(NewButton, Row, Col);
		}

		Col++;
		if (Col >= MaxColumns)
		{
			Col = 0;
			Row++;
		}
	}
}

void USkillWidget::UpdateButtons(int32 currentAp)
{
	int32 NumChildren = SkillGridPanel->GetChildrenCount();

	for (int32 i = 0; i < NumChildren; ++i)
	{
		UWidget* Child = SkillGridPanel->GetChildAt(i);
		if (USkillButtonWidget* SkillButton = Cast<USkillButtonWidget>(Child))
		{
			SkillButton->SetIsEnabled(currentAp >= SkillButton->skillAp);
		}
	}
}

void USkillWidget::SetEndButton(bool visibility)
{
	if (visibility)
	{
		EndButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		EndButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}
