// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMapManager::AMapManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create scenecomponent for root
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);

	// Create spring arm component
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	// Create and initialize the SceneCaptureComponent
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(SpringArmComponent);

	//initialize the SceneCaptureComponent
	InitializeSceneCaptureComponent();

	//Capture the map texture
	CaptureMap();

}

// Called when the game starts or when spawned
void AMapManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapManager::InitializeSceneCaptureComponent()
{

	SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCaptureComponent->OrthoWidth = MapDimensions.X;
	SceneCaptureComponent->bCaptureEveryFrame = false;
	SceneCaptureComponent->TextureTarget = RTMapTexture;
}

void AMapManager::CaptureMap()
{
	UE_LOG(LogTemp, Warning, TEXT("CaptureMap function called hhi!"));
	InitializeSceneCaptureComponent();
	SceneCaptureComponent->CaptureScene();
}

