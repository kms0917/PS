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
		if (Buff->buffCount <= 0)
		{
			ownerCharacter->hp -= Buff->hp;
			ownerCharacter->currentStr -= Buff->str;
			ownerCharacter->currentMag -= Buff->mag;
			ownerCharacter->currentDef -= Buff->def;
			ownerCharacter->currentRes -= Buff->res;
			ownerCharacter->currentSkill -= Buff->skill;
			ownerCharacter->currentSpeed -= Buff->speed;
			ownerCharacter->currentMoveSpeed -= Buff->moveSpeed;
			ownerCharacter->currentAp -= Buff->ap;
			ownerCharacter->damageReduction -= Buff->damageReduction;
			ownerCharacter->damageReduction_Percent -= Buff->damageReduction_Percent;
			ownerCharacter->damageReinforcement -= Buff->damageReinforcement;
			ownerCharacter->damageReinforcement_Percent -= Buff->damageReinforcement_Percent;
		}
	}
	// 조건에 맞는 항목들을 한 번에 제거
	int32 RemovedCount = buffList.RemoveAll([](const UBuffBase* Buff)
	{
		return !IsValid(Buff) || Buff->buffCount <= 0;
	});
    
	if (RemovedCount > 0)
	{
		CalcBuffStats();
		if (ownerCharacter)
		{
			ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
			ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
			ownerCharacter->critical = ownerCharacter->CalcCritical(0);
			ownerCharacter->SetHealthWidget();
		}
	}
}

//버프 추가, 만약 버프를 얻었을때가 전투중이 아니라면 타이머 작동
void UBuffComponent::GetBuff(UBuffBase* Buff)
{
	UBuffBase* newbuff = Buff->DuplicateBuff(NewObject<UBuffBase>(this, UBuffBase::StaticClass(), Buff->GetFName()));
	if (!buffList.Contains(newbuff))	//새로운 버프가 추가될경우 list에 추가 및 턴 갱신, 같은 이름의 버프가 들어오면 위의 newBuff는 이전의 버프 포인터를 반환하고,Duplicate에서 Couont 리셋해줌
	{
		buffList.Add(newbuff);
		if (ownerCharacter)
		{
			ownerCharacter->currentHp += newbuff->hp;
			ownerCharacter->hp += newbuff->hp;
			ownerCharacter->currentStr += newbuff->str;
			ownerCharacter->currentMag += newbuff->mag;
			ownerCharacter->currentDef += newbuff->def;
			ownerCharacter->currentRes += newbuff->res;
			ownerCharacter->currentSkill += newbuff->skill;
			ownerCharacter->currentSpeed += newbuff->speed;
			ownerCharacter->currentMoveSpeed += newbuff->moveSpeed;
			ownerCharacter->currentAp += newbuff->ap;
			ownerCharacter->damageReduction += newbuff->damageReduction;
			ownerCharacter->damageReduction_Percent += newbuff->damageReduction_Percent;
			ownerCharacter->damageReinforcement += newbuff->damageReinforcement;
			ownerCharacter->damageReinforcement_Percent += newbuff->damageReinforcement_Percent;
			ownerCharacter->accuracy = ownerCharacter->CalcAccuracy(0);
			ownerCharacter->evasion = ownerCharacter->CalcEvasion(0);
			ownerCharacter->critical = ownerCharacter->CalcCritical(0);
			ownerCharacter->SetHealthWidget();
		}
	}
	CalcBuffStats();
	if (ownerCharacter && !ownerCharacter->bIsBattle)
	{
		StartBuffTimer();
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

void UBuffComponent::StartBuffTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
			buffCountReduceTimer,
			this,
			&UBuffComponent::ReduceBuffCount,
			5.0f,
			true
			);
}

//모든 버프들의 총 스탯을 계산, 스탯 상세보기 위젯에서 보여주기 위함, 실제 스탯 반영x
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
}

