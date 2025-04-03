// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

class ACharacterBase;
class UUniformGridPanel;
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

public:
	void UpdateWidget(ACharacterBase* ControlledCharacter);

private:
	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	UUniformGridPanel* SkillGridPanel;
};
