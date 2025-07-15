// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuffComponent.generated.h"

class UBuffBase;
class ACharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PS_API UBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBuffComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	//아래 스탯들은 스탯 상세보기 지원을 위해 저장, 실제 스탯에 반영x
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	float buffedMoveSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedAp;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedHp;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedDef;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedRes;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedStr;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedMag;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedSkill;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedCritical;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedAccuracy;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedEvade;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	float buffedDamageReduction_Percent;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedDamageReduction;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	float buffedDamageReinforcement_Percent;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 buffedDamageReinforcement;

	void ReduceBuffCount();
	void GetBuff(UBuffBase* Buff);
	void StopBuffTimer();
	void StartBuffTimer();

private:
	void CalcBuffStats();

	UPROPERTY(EditAnywhere)
	TArray<UBuffBase*> buffList;

	UPROPERTY(EditAnyWhere)
	ACharacterBase* ownerCharacter;
	
	FTimerHandle buffCountReduceTimer;
};
