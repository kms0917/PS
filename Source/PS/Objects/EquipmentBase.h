// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EquipmentBase.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class PS_API UEquipmentBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 hp = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 str = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 mag = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 def = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 res = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 skill = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 speed = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	float moveSpeed = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 damage = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 accuracy = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 evasion = 0; 
	
	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 critical = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	int32 ap = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly)
	ACharacterBase* ownerCharacter;
	
};
