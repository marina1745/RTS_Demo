// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectableInterface.h"
#include "CLMBaseBuilding.generated.h"


class UStaticMeshComponent;
class UFloatingPawnMovement;

UCLASS()
class TOPDOWN_UTILITIES_API ACLMBaseBuilding : public AActor, public ISelectableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACLMBaseBuilding();


	//Skeletal Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SkeletalMesh;

	
	//Floating Pawn movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Building, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SelectedIndicator;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SelectActor_Implementation(const bool Select) override;

};
