// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class UWeaponBase;
class UHelmetBase;
class UChestBase;
class UGloveBase;
class ULegBase;
class UBootsBase;
class ACharacterBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PS_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnyWhere, Transient, Category = "Equipment")
	UWeaponBase* weapon = nullptr;
	UPROPERTY(EditAnyWhere, Transient, Category = "Equipment")
	UHelmetBase* helmet = nullptr;
	UPROPERTY(EditAnyWhere, Transient, Category = "Equipment")
	UChestBase* chest = nullptr;
	UPROPERTY(EditAnyWhere, Transient, Category = "Equipment")
	UGloveBase* glove = nullptr;
	UPROPERTY(EditAnyWhere, Transient, Category = "Equipment")
	ULegBase* leg = nullptr;
	UPROPERTY(EditAnyWhere, Transient, Category = "Equipment")
	UBootsBase* boots = nullptr;

	//아래의 값들은 위젯에서 상세보기를 지원하기 위해 저장, 실제 스탯에는 영향x
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentHp = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentStr = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentMag = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentDef = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentRes = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentSkill = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentSpeed = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	float equipmentMoveSpeed = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentDamage = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentAccuracy = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentEvasion = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentCritical = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 equipmentAp = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	int32 equipmentDamageReduction = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	float equipmentDamageReduction_percent = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	int32 equipmentDamageReinforcement = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	float equipmentDamageReinforcement_percent = 0;
		
	void EquipWeapon(UWeaponBase* EquipedWeapon);
	void EquipHelmet(UHelmetBase* EquipedHelmet);
	void EquipChest(UChestBase* EquipedChest);
	void EquipGlove(UGloveBase* EquipedGlove);
	void EquipLeg(ULegBase* EquipedLeg);
	void EquipBoots(UBootsBase* EquipedBoots);

	void UnEquipWeapon();
	void UnEquipHelmet();
	void UnEquipChest();
	void UnEquipGlove();
	void UnEquipLeg();
	void UnEquipBoots();

	void CalcEquipmentStats();

private:
	UPROPERTY(EditAnyWhere)
	ACharacterBase* ownerCharacter;
};
