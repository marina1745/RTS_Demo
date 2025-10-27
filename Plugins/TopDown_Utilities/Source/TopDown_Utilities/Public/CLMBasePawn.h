// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SelectableInterface.h"
#include "NavigableInterface.h"
#include "CLMBasePawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UFloatingPawnMovement;

UCLASS()
class TOPDOWN_UTILITIES_API ACLMBasePawn : public APawn, public ISelectableInterface, public INavigableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACLMBasePawn();

private:
	//Capsule Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	//Skeletal Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	//Floating Pawn movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;

	//Floating Pawn movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SelectedIndicator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	float maxSpeed=100.f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move();
	FVector TargetLocation = FVector::ZeroVector;
	bool isMoving = false;
	float AcceptanceDistance = 200.f;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	UFUNCTION()
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
	void SelectActorLocal(const bool Select);

	void SelectActor_Implementation(const bool Select) override;

	void CommandActor_Implementation(const FVector Target) override;
};
