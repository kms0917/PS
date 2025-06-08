// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DoAIDamage.generated.h"

/**
 * 
 */
UCLASS()
class PS_API UAnimNotify_DoAIDamage : public UAnimNotify
{
	GENERATED_BODY()

public:
	// Called when notify is triggered
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
