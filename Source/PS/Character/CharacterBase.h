// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "CharacterBase.generated.h"

class UHealthWidget;
class USkillInfoWidget;
class ACharacterController;
class UEquipmentComponent;
class USkillComponent;
class USkillBase;

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

	void SetStats();			//첫 생성시와 매 턴 시작시 호출
	void SetSkillInfo();		//첫 생성시와 매 턴 시작시 호출
	int CalcCritical(int correction);	//CalcDMG까지 스킬 사용 시 호출
	int CalcEvasion(int correction);
	int CalcAccuracy(int correction);
	int CalcDamage(int damage, float magnification, bool isMag);
	void LevelUp();		//GetEXP 후 호출

	virtual void SetDefaultEquipments();
	virtual void SetDefaultSkills();

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

	void OnSkillAutoMoveFinished(FVector attackPoint);
	bool IsMontagePlayed();

	void UseSkill(int i);
	void ReflectDamage();	//위의 스킬들 위젯에서 누르면 스킬에서 데미지,명중률 등 계산 후 위젯에 넘기고 위젯에서 대상 선택시 대상스탯으로 계산해 finalDamage소유, 이 함수로 실제 실행되면 값 넘겨주기만함
	void GetEXP();	//아군용
	void SetLevel(int32 levelScaleAmount);	//적군용
	void TargettedOn(int32 accuracyRate, int32 criticalRate, int32 Damage, bool isMag);
	void TargettedOff();

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	float moveSpeed = 10;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	float currentMoveSpeed;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 ap = 2;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stats")
	int32 currentAp;
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
	int32 savedDamage = 0;		//targetted 됐을때 공격이 일어나면 사용할 실제 받을 데미지 값
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	int32 savedAccuracy = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	int32 savedCritical = 0;

	UPROPERTY(EditAnyWhere, Transient)
	UEquipmentComponent* equipmentComponent;
	UPROPERTY(EditAnyWhere, Transient)
	USkillComponent* skillComponent;

	UPROPERTY()
	UWidgetComponent* skillInfoWidgetComponent;					//공격 범위 안에 들어갈 시 공격 정보를 띄움
	UPROPERTY()
	USkillInfoWidget* skillInfoWidget;

	UPROPERTY()
	bool bIsTargeted = true;

	UPROPERTY()
	USkillBase* currentUsedSkill;
	UPROPERTY()
	bool bDidApplyDamage = false;

private:
	void UpdateWidgetRotation();
	void UpdateSkillInfoWidgetLocation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* cameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* springArmComponent;

	ACharacterController* playerController;

	UPROPERTY()
	UWidgetComponent* healthWidgetComponent;		//체력 및 남은 턴수 표시

	UPROPERTY()
	UHealthWidget* healthWidget;	//위의 위젯을 캐스팅해서 담을 변수
};
