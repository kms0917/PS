// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MovePoint.h"
#include "Widget/DistanceWidget.h"
#include "Controller/CharacterController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMovePoint::AMovePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(SphereComponent);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("WidgetBlueprint'/Game/Widget/W_DistanceWidget'"));
	if (WidgetClass.Succeeded())
	{
		WidgetComponent->SetWidgetClass(WidgetClass.Class);  // BP로 만든 위젯을 설정
	}
	WidgetComponent->SetDrawSize(FVector2D(300, 100));
}

// Called when the game starts or when spawned
void AMovePoint::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<ACharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

}

// Called every frame
void AMovePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = PlayerController->cameraRotation;
	NewRotation.Yaw += 180.0f;
	NewRotation.Pitch += 120.0f;
	WidgetComponent->SetWorldRotation(NewRotation);

}

