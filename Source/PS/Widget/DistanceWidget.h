// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DistanceWidget.generated.h"

class ACharacterController;
class ANormalGameMode;

/**
 * 
 */
UCLASS()
class PS_API UDistanceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DistanceText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ReachableText;

private:
    ACharacterController* playerController;

    ANormalGameMode* gameMode;
};
