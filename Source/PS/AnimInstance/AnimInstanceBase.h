// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class PS_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UAnimInstanceBase();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(VisibleAnyWhere, BLueprintReadOnly)
	ACharacterBase* OwnerCharacter;

	UPROPERTY(VisibleAnyWhere, BLueprintReadOnly)
	float speed;

	UPROPERTY(VisibleAnyWhere, BLueprintReadOnly)
	bool bIsInAir;
};
