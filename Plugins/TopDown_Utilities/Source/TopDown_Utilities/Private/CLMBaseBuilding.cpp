// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMBaseBuilding.h"
#include "Components/BoxComponent.h"

// Sets default values
ACLMBaseBuilding::ACLMBaseBuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Skeletal mesh 
	SkeletalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	
	RootComponent = SkeletalMesh;
	
	
	// Create selected indicator
	SelectedIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectedIndicator"));
	SelectedIndicator->SetupAttachment(RootComponent);
	SelectedIndicator->SetHiddenInGame(true);
	SelectedIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

// Called when the game starts or when spawned
void ACLMBaseBuilding::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACLMBaseBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACLMBaseBuilding::SelectActor_Implementation(const bool Select)
{
	SelectedIndicator->SetHiddenInGame(!Select);
}

