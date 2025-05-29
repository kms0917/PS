// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillIndicator.generated.h"

class ACharacterBase;

UCLASS()
class PS_API ASkillIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillIndicator();

	void SetSkillIndicator(int32 accuracy, int32 critical, int32 damage, bool isMag, float attackRange);
	void InitAttack();
	void SetUnvisible();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere)
	TArray<ACharacterBase*> overlappedCharacters;

	int32 damage;
	int32 accuracy;
	int32 critical;
	bool bIsMag;

	ACharacterBase* playerCharacter;

private:	
	UFUNCTION()
	void OverlapWithCharacter(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
