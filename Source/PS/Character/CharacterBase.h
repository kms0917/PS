// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "CharacterBase.generated.h"

class UHealthWidget;
class ACharacterController;
class UEquipmentComponent;

UCLASS()
class PS_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetStats();

	void CalcCritical(int correction);	//CalcDMG까지 스킬 사용 시 호출
	void CalcEvasion(int correction);
	void CalcAccuracy(int correction);
	void CalcDMG(int correction);
	void LevelUp();		//GetEXP 후 호출

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 hpGrowth = 10;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 strGrowth = 10;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 magGrowth = 10;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 defGrowth = 10;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 resGrowth = 10;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 skillGrowth = 10;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 speedGrowth = 10;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Skill1();
	virtual void Skill2();
	virtual void Skill3();
	virtual void Skill4();
	virtual void Skill5();		//더 많이 추가
	
	void ReflectDamage(int finalDamage, float acurracyRate, float criticalRate);	//위의 스킬들 위젯에서 누르면 스킬에서 데미지,명중률 등 계산 후 위젯에 넘기고 위젯에서 대상 선택시 대상스탯으로 계산해 finalDamage소유, 이 함수로 실제 실행되면 값 넘겨주기만함
	void GetEXP();	//아군용
	void SetLevel();	//적군용

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	float moveSpeed = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	float currentMoveSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 ap;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 hp = 100;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 currentHp;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 str = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 currentStr = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 mag = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 currentMag = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 def = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 currentDef = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 res = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 currentRes = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 skill = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 currentSkill = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 speed = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 currentSpeed = 10;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 exp = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 level = 1;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	int32 accuracy;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	int32 evasion;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	int32 critical;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	int32 damage;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	bool bIsMag = false;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* cameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* springArmComponent;

	UPROPERTY(VisibleAnywhere)
	UEquipmentComponent* equipmentComponent;

	ACharacterController* playerController;

	UPROPERTY()
	UWidgetComponent* widgetComponent;		//체력 및 남은 턴수 표시

	UPROPERTY()
	UHealthWidget* healthWidget;	//위의 위젯을 캐스팅해서 담을 변수
};
