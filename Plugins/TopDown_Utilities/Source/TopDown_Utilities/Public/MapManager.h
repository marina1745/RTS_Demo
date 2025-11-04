// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapManager.generated.h"

class USceneCaptureComponent2D;
class USpringArmComponent;

UCLASS()
class TOPDOWN_UTILITIES_API AMapManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMapManager();

private:
	// Map dimesnions
	UPROPERTY(EditAnywhere, Category = "Map Settings")
	FVector2D MapDimensions = FVector2D(70000.0f, 70000.0f);

	// Map capture height
	UPROPERTY(EditAnywhere, Category = "Map Settings")
	float MapCaptureHeight = 8000.0f;

	// SceneCaptureComponent to capture the map texture
	UPROPERTY(EditAnywhere, Category = "Map Settings")
	USceneCaptureComponent2D* SceneCaptureComponent;

	// Texture to store the map
	UPROPERTY(EditAnywhere, Category = "Map Settings")
	UTextureRenderTarget2D* RTMapTexture;

	// Spring arm component for the SceneCaptureComponent
	UPROPERTY(EditAnywhere, Category = "Map Settings")
	USpringArmComponent* SpringArmComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Initialize the SceneCaptureComponent
	void InitializeSceneCaptureComponent();

	// Function to capture the map texture
	UFUNCTION(BlueprintCallable, Category = "Map", CallInEditor)
	void CaptureMap();

};
