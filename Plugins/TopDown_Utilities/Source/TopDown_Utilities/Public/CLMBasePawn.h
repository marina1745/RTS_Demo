// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SelectableInterface.h"
#include "NavigableInterface.h"
#include "BasePawnInterface.h"
#include "CommonEnums.h"
#include "CLMBasePawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UFloatingPawnMovement;




UCLASS()
class TOPDOWN_UTILITIES_API ACLMBasePawn : public APawn, public ISelectableInterface, public INavigableInterface, public IBaseActorInterface
{
	GENERATED_BODY()
public:
	ACLMBasePawn();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
	EUnitRole UnitRole = EUnitRole::Melee;

	// World-space spacing between formation slots
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
	float FormationSpacing = 160.f;

	// How many units per row for this pawn type
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Formation")
	int32 UnitsPerRow = 5;

	

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
	float AcceptanceDistance = 100.f;


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
	
	EUnitRole GetActorType_Implementation() override;
};
