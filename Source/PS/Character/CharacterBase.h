// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "GenericTeamAgentInterface.h"
#include "CharacterBase.generated.h"

class UHealthWidget;
class USkillInfoWidget;
class ACharacterController;
class UEquipmentComponent;
class USkillComponent;
class USkillBase;
class UAIPerceptionStimuliSourceComponent;
class UMaterialInterface;
class UBuffComponent;

UCLASS()
class PS_API ACharacterBase : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void SetSkillInfo();		//첫 생성시와 매 턴 시작시 호출
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

	FGenericTeamId TeamId; // 기본 무소속

	UPROPERTY(EditDefaultsOnly, Category = "Effects|Material")
	UMaterialInterface* OverlayMaterial;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnSkillAutoMoveFinished(FVector attackPoint);
	bool IsMontagePlayed();

	void SetStats();			//첫 생성시 호출
	int CalcCritical(int correction);	//CalcDMG까지 스킬 사용 시 호출
	int CalcEvasion(int correction);
	int CalcAccuracy(int correction);
	void SetHealthWidget();
	void RestoreApandMoveSpeed();
	void UseSkill(int i);
	void ReflectDamage(bool isHeal);	//위의 스킬들 위젯에서 누르면 스킬에서 데미지,명중률 등 계산 후 위젯에 넘기고 위젯에서 대상 선택시 대상스탯으로 계산해 finalDamage소유, 이 함수로 실제 실행되면 값 넘겨주기만함
	void GetEXP();	//아군용
	void SetLevel(int32 levelScaleAmount);	//적군용
	void TargettedOn(int32 accuracyRate, int32 criticalRate, int32 Damage, bool isMag, bool isEnemy, bool isHeal);
	void TargettedOff();
	void SetTurnText(int32 turn);
	void TurnStart();
	void TurnEnd();

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamId; }
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override { TeamId = NewTeamId; }

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

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	float damageReduction_Percent = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	int32 damageReduction = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	float damageReinforcement_Percent = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Combat")
	int32 damageReinforcement = 0;

	UPROPERTY(EditAnyWhere, Transient)
	UEquipmentComponent* equipmentComponent;
	UPROPERTY(EditAnyWhere, Transient)
	USkillComponent* skillComponent;
	UPROPERTY(EditAnyWhere, Transient)
	UBuffComponent* buffComponent;
	
	UPROPERTY()
	UWidgetComponent* skillInfoWidgetComponent;					//공격 범위 안에 들어갈 시 공격 정보를 띄움
	UPROPERTY()
	USkillInfoWidget* skillInfoWidget;

	UPROPERTY()
	bool bIsTargeted = true;

	UPROPERTY()
	bool bMyTurn = false;

	UPROPERTY()
	bool bIsBattle = false;

	UPROPERTY()
	USkillBase* currentUsedSkill;

	ACharacterController* playerController;

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void SetOverlayMaterialEnabled(bool bEnable);

	
private:
	void UpdateWidgetRotation();
	void UpdateSkillInfoWidgetLocation();
	void SetBPs();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* cameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* springArmComponent;

	UPROPERTY()
	UWidgetComponent* healthWidgetComponent;		//체력 및 남은 턴수 표시

	UPROPERTY()
	UHealthWidget* healthWidget;	//위의 위젯을 캐스팅해서 담을 변수

	UPROPERTY(VisibleAnywhere)
	UAIPerceptionStimuliSourceComponent* StimuliSource;
};
