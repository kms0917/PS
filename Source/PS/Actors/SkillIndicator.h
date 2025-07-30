// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "SkillIndicator.generated.h"

class ACharacterBase;

UCLASS()
class PS_API ASkillIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillIndicator();

	virtual void Tick(float DeltaTime) override;

	void SetSkillIndicator(int32 accuracy, int32 critical, int32 damage, bool isMag, float attackRange, bool IsHeal);
	void InitAttack();
	void SetUnvisible();

	UPROPERTY(EditAnywhere)
	TArray<ACharacterBase*> overlappedCharacters;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* WidgetComponent;

	class USkillIndicatorWidget* usableWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* OverlapSphere;

	UPROPERTY()
	UDecalComponent* DecalComponent;

	int32 damage;
	int32 accuracy;
	int32 critical;
	bool bIsMag;
	bool IsHeal;

	ACharacterBase* playerCharacter;

private:
	class ACharacterController* playerController;

};
