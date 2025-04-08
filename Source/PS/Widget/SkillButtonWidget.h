// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillButtonWidget.generated.h"

DECLARE_DELEGATE_OneParam(FOnSkillButtonClicked, int32);

class USkillBase;
/**
 * 
 */
UCLASS()
class PS_API USkillButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnSkillButtonClicked OnSkillButtonClicked;

	void SettingButton(USkillBase* Skill, int32 index);

	virtual void NativeDestruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* skillButton;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* skillNameText;

	UFUNCTION()
	void HandleButtonClicked();

	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class USkillTooltipWidget> TooltipWidgetClass;		//BP에서 설정

private:
	int32 skillIndex;
};
