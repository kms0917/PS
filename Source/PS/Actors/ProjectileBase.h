// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Controller/AIController/BasicAIController.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class USkillBase;
class ACharacterController;

UCLASS()
class PS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComp, FVector NormalImpulse, 
						 const FHitResult& Hit);

	void DestroyProjectile();
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraComponent* trailEffectComponent;		//하위 클래스에서 바꿔서 사용
	
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* projectileEffect;				//발사체가 사라질때 출력하는 이펙트, 하위 클래스에서 바꿔서 사용
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float ProjectileSpeed = 1000.0f;				//하위 클래스에서 바꿔서 사용

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetProjectile(AController* playerController, USkillBase* skill, AActor* targetActor);

private:
	AActor* targetActor;
	ACharacterController* playerController;
	USkillBase* skill;
	bool bIsEnemy;
	ABasicAIController* AICon;
	
};
