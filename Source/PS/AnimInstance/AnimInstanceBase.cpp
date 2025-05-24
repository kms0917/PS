// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/AnimInstanceBase.h"
#include "Character/CharacterBase.h"

#include "GameFrameWork/CharacterMovementComponent.h"

UAnimInstanceBase::UAnimInstanceBase()
{
}

void UAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacterBase>(GetOwningActor());
}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (OwnerCharacter)
	{
		bIsInAir = OwnerCharacter->GetMovementComponent()->IsFalling();
		speed = OwnerCharacter->GetVelocity().Size();
	}
}
