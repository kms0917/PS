// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

class ACharacterBase;
class UUniformGridPanel;
class USkillButtonWidget;

/**
 * 
 */
UCLASS()
class PS_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	ACharacterBase* currentCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USkillButtonWidget> SkillButtonClass;


public:
	void UpdateWidget(ACharacterBase* ControlledCharacter);

	void UpdateButtons(int32 currentAp);

	void SetEndButton(bool visibility);

private:
	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	UUniformGridPanel* SkillGridPanel;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	class UButton* EndButton;
};
