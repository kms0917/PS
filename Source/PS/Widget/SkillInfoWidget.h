// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillInfoWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class PS_API USkillInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SettingWidget(int32 accuracy, int32 critical, int32 damage);		//skillIndicator에서 최종 계산 후 계산된 값을 넘겨줌

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* accuracyText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* criticalText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* damageText;

};
