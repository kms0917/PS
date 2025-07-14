// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/BuffComponent.h"
#include "Character/CharacterBase.h"
#include "Objects/Buff/BuffBase.h"

// Sets default values for this component's properties
UBuffComponent::UBuffComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* ownerActor = GetOwner();

	if (ownerActor)
	{
		ACharacterBase* MyCharacter = Cast<ACharacterBase>(ownerActor);
		if (MyCharacter)
		{
			ownerCharacter = MyCharacter;
		}
	}
	CalcBuffStats();
}

//버프 지속시간 1씩 줄인 후 지속시간 다 된 버프 삭제 후 CalcBuffStats 호출
void UBuffComponent::ReduceBuffCount()
{
	if (buffList.Num() == 0 && GetWorld()->GetTimerManager().IsTimerActive(buffCountReduceTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(buffCountReduceTimer);
	}
	for (UBuffBase* Buff : buffList)
	{
		Buff->buffCount -= 1;
		UE_LOG(LogTemp, Warning, TEXT("Remain Buff Count : %d, Owner : %s"), Buff->buffCount, *ownerCharacter->GetName());
	}
	// 조건에 맞는 항목들을 한 번에 제거
	int32 RemovedCount = buffList.RemoveAll([](const UBuffBase* Buff)
	{
		return !IsValid(Buff) || Buff->buffCount <= 0;
	});
    
	if (RemovedCount > 0)
	{
		CalcBuffStats();
	}
}

//버프 추가, 만약 버프를 얻었을때가 전투중이 아니라면 타이머 작동
void UBuffComponent::GetBuff(UBuffBase* Buff)
{
	UBuffBase* newbuff = NewObject<UBuffBase>(this, UBuffBase::StaticClass(), Buff->GetFName());
	buffList.Add(Buff->DuplicateBuff(newbuff));
	CalcBuffStats();
	if (ownerCharacter && !ownerCharacter->bIsBattle)
	{
		GetWorld()->GetTimerManager().SetTimer(
			buffCountReduceTimer,
			this,
			&UBuffComponent::ReduceBuffCount,
			5.0f,
			true
			);
	}
}

//전투중이 아닐때 버프는 타이머로 자동으로 턴이 감소하도록함, 전투가 시작될 시 해당 타이머를 멈추는 함수
void UBuffComponent::StopBuffTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(buffCountReduceTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(buffCountReduceTimer);
	}
}

//모든 버프들의 총 스탯을 계산
void UBuffComponent::CalcBuffStats()
{
	buffedMoveSpeed = 0.0f;
	buffedHp = 0;
	buffedDef = 0;
	buffedRes = 0;
	buffedStr = 0;
	buffedMag = 0;
	buffedSkill = 0;
	buffedSpeed = 0;
	buffedAp = 0;
	buffedCritical = 0;
	buffedAccuracy = 0;
	buffedEvade = 0;
	buffedDamageReduction_Percent = 0.0f;
	buffedDamageReduction = 0;
	buffedDamageReinforcement_Percent = 0.0f;
	buffedDamageReinforcement = 0;
	
	for (UBuffBase* Buff : buffList)
	{
		if (Buff)
		{
			buffedMoveSpeed += Buff->moveSpeed;
			buffedHp += Buff->hp;
			buffedDef += Buff->def;
			buffedRes += Buff->res;
			buffedStr += Buff->str;
			buffedMag += Buff->mag;
			buffedSkill += Buff->skill;
			buffedSpeed += Buff->speed;
			buffedAp += Buff->ap;
			buffedCritical += Buff->critical;
			buffedAccuracy += Buff->accuracy;
			buffedEvade += Buff->evade;
			buffedDamageReduction_Percent += Buff->damageReduction_Percent;
			buffedDamageReduction += Buff->damageReduction;
			buffedDamageReinforcement_Percent += Buff->damageReinforcement_Percent;
			buffedDamageReinforcement += Buff->damageReinforcement;
		}
	}
	if (ownerCharacter)
	{
		ownerCharacter->SetStats(false);
	}
}

