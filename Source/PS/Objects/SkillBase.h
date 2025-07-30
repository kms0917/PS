// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "SkillBase.generated.h"

class ACharacterBase;
class UBuffBase;
class AProjectileBase;
/**
 * 
 */
UCLASS()
class PS_API USkillBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<ACharacterBase*> overlappedCharacters;

	UPROPERTY()
	UAnimMontage* skillMontage;		//하위 클래스인 스킬 인스턴스에서 할당

	UPROPERTY()
	float PlayRate = 1.0f;

	UPROPERTY()
	UBuffBase* buff = nullptr;
	
	UFUNCTION()
	void ApllyDamage();

	UPROPERTY(EditAnyWhere, Category = "Skill")
	float skillRange = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	float attackRange = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	int32 apUsage = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	int32 accuracy = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	int32 critical = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	int32 damage = 0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	float magnification = 1.0;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	bool bIsMag = false;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	bool bIsHeal = false;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	bool bIsBuff = false;

	UPROPERTY(EditAnyWhere, Category = "Skill")
	bool bIsTargeting = false;
	
	UPROPERTY(EditAnyWhere, Category = "Skill")
	int32 multiTargetingNum = 3;

	UPROPERTY(EditAnyWhere, Category = "Script")
	FText skillName = FText::FromString(" ");

	UPROPERTY(EditAnyWhere, Category = "Script")
	FText skillScript = FText::FromString(" ");

	UPROPERTY(EditAnyWhere, Category = "Combat")
	int32 calculatedDamage = 0;

	UPROPERTY(EditAnyWhere, Category = "Combat")
	int32 calculatedAccuracy = 0;

	UPROPERTY(EditAnyWhere, Category = "Combat")
	int32 calculatedCritical = 0;

	UPROPERTY(EditAnywhere, Category = "Effect")
	TSubclassOf<AProjectileBase> projectile = nullptr;		//스킬이펙트가 있거나 이게 있거나 해야 함

	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* selfEffectParticle;			//스킬 사용자에게 생기는 이펙트, skillmode에 들어가면 사용

	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* skillEffectParticle;		//스킬이펙트
};
