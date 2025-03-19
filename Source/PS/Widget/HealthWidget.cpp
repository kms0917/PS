// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/HealthWidget.h"

void UHealthWidget::SetHealthBar(int currnetHp, int maxHp)		//위젯을 소유한 폰에서 호출
{
	if (healthBar)
	{
		float percentage = static_cast<float>(currnetHp) / static_cast<float>(maxHp);
		healthBar->SetPercent(percentage);
	}
	if (healthBar)
	{
		FText HealthTextFormat = FText::Format(NSLOCTEXT("YourNamespace", "HealthFormat", "{0} / {1}"), FText::AsNumber(currnetHp), FText::AsNumber(maxHp));

		healthText->SetText(HealthTextFormat);
	}
}

void UHealthWidget::SetTurnText(int turn)			//위젯을 소유한 폰에서 턴이 바뀔 시 호출, 델리게이트 or 게임모드에서 턴 바뀔때 관리
{
	if (turn == -1)
	{
		turnText->SetText(FText::FromString(" "));
	}
	else
	{
		turnText->SetText(FText::AsNumber(turn));
	}
}
