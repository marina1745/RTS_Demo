// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownHUD.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_UTILITIES_API ATopDownHUD : public AHUD
{
	GENERATED_BODY()

private:
	bool bDrawRect = false;
	bool bSelectActor = false;
	FVector2D SelectStart;
	FVector2D SelectSize;
	FLinearColor SelectionColor = FLinearColor(1.f, 0, 0, 0.2f);
	TArray<AActor*>  SelectedActors;
protected:
	virtual void DrawHUD() override;
public:
	void ShowSelection(const FVector2D start,const FVector2D size);
	void HideSelection();
	TArray<AActor*> GetSelectedActors();
};
