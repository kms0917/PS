// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ProjectileBase.h"
#include "Controller/CharacterController.h"
#include "Objects/SkillBase.h"

#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(10.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnProjectileHit);

	trailEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailComponent"));
	trailEffectComponent->SetupAttachment(RootComponent);
	trailEffectComponent->SetSystemFixedBounds(FBox(FVector(-500, -500, -500), FVector(500, 500, 500)));
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileBase::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	DestroyProjectile();
}

void AProjectileBase::DestroyProjectile()
{
	if (bIsEnemy)
	{
		if (projectileEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), projectileEffect, this->GetActorLocation(), this->GetActorRotation());
		}
		AICon->DoAIDamage();
		trailEffectComponent->Deactivate();
		Destroy();
	}
	else
	{
		if (projectileEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), projectileEffect, this->GetActorLocation(), this->GetActorRotation());
		}
		trailEffectComponent->Deactivate();
		playerController->InitAttack();
		skill->ApllyDamage();
		Destroy();
	}
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector CurrentLocation = GetActorLocation();

	if (FVector::DistSquared(CurrentLocation, targetActor->GetActorLocation()) < 100)
	{
		DestroyProjectile();
	}
	
	if (targetActor)	//이펙트의 방향 제어도 필요
	{
		FVector Destination = targetActor->GetActorLocation();
		FVector Direction = (Destination - CurrentLocation).GetSafeNormal();
		FVector NewLocation = CurrentLocation + Direction * ProjectileSpeed * DeltaTime;
		SetActorLocation(NewLocation);
	}
}

void AProjectileBase::SetProjectile(AController* PlayerController, USkillBase* Skill, AActor* TargetActor)
{

	this->playerController = Cast<ACharacterController>(PlayerController);
	if (!this->playerController)
	{
		this->AICon = Cast<ABasicAIController>(PlayerController);
		this->bIsEnemy = true;
	}
	else
	{
		this->bIsEnemy = false;
	}
	this->skill = Skill;
	this->targetActor = TargetActor;
}