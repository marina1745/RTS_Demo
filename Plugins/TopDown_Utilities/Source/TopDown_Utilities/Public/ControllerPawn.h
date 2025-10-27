// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ControllerPawn.generated.h"

class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UFloatingPawnMovement;

UCLASS()
class TOPDOWN_UTILITIES_API AControllerPawn : public APawn
{
	GENERATED_BODY()

private:

	//Capsule Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Collision, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArm;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	/** CameraProperties */
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CameraZoomSpeed = 10.f;
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MaxCameraOrthoWidth = 2500.f;
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float MinCameraOrthoWidth = 300.f;


	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Zoom Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* FloatingPawnMovement;

public:
	// Sets default values for this pawn's properties
	AControllerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for Zoom input */
	void Zoom(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	UFUNCTION()
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
