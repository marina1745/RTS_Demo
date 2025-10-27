// Fill out your copyright notice in the Description page of Project Settings.


#include "CLMBasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AIController.h"
#include <GameFramework/Character.h>

// Sets default values
ACLMBasePawn::ACLMBasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create capsule
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	//Create Skeletal mesh 
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);

	//Create floating pawn movement
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));


	if (UFloatingPawnMovement* Move = Cast<UFloatingPawnMovement>(GetMovementComponent()))
	{
		Move->MaxSpeed = maxSpeed;
		Move->Acceleration = 800.f;
		Move->Deceleration = 800.f;
	}

	// Create selected indicator
	SelectedIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectedIndicator"));
	SelectedIndicator->SetupAttachment(RootComponent);
	SelectedIndicator->SetHiddenInGame(true);
	SelectedIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision);



}

// Called when the game starts or when spawned
void ACLMBasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void ACLMBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}

// Called to bind functionality to input
void ACLMBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACLMBasePawn::SelectActorLocal(const bool Select)
{
	SelectedIndicator->SetHiddenInGame(!Select);
}

void ACLMBasePawn::SelectActor_Implementation(const bool Select)
{
	SelectedIndicator->SetHiddenInGame(!Select);
}

void ACLMBasePawn::CommandActor_Implementation(const FVector Target)
{
	isMoving = true;
	TargetLocation = Target;
	UE_LOG(LogTemp, Display, TEXT("commanded pawn to move to "));
	AAIController* ai_controller = Cast<AAIController>(GetController());
	ai_controller->MoveToLocation(TargetLocation,AcceptanceDistance);

}
void ACLMBasePawn::Move()
{
	if (isMoving)
	{
		FVector moveDirection = TargetLocation - GetActorLocation();
		if (moveDirection.Length() < AcceptanceDistance)
		{
			isMoving = false;
			return;
		}
		//moveDirection.Normalize(1);
		//AddMovementInput(moveDirection, 0.5f);
		FRotator rotation = UKismetMathLibrary::MakeRotFromX(moveDirection);
		rotation.Pitch = 0;
		rotation.Roll = 0;
		FRotator actualRotation = FMath::RInterpTo(GetActorRotation(), rotation, GetWorld()->GetDeltaSeconds() , 5.f);
		SetActorRotation(actualRotation);
	}
}

