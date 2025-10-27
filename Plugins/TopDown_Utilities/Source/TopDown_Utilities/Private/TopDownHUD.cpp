// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownHUD.h"

void ATopDownHUD::DrawHUD()
{
	if (bDrawRect)
	{
		DrawRect(SelectionColor, SelectStart.X, SelectStart.Y, SelectSize.X, SelectSize.Y);
		//UE_LOG(LogTemp, Display, TEXT("Drawing"));
	}
	if (bSelectActor)
	{
		SelectedActors.Empty();
		GetActorsInSelectionRectangle<AActor>(SelectStart, SelectStart + SelectSize, SelectedActors, false, false);
		bSelectActor = false;
	}
}

void ATopDownHUD::ShowSelection(const FVector2D start, const FVector2D size)
{
	bDrawRect = true;
	SelectStart = start;
	SelectSize = size;
}
void ATopDownHUD::HideSelection()
{
	bDrawRect = false;
	bSelectActor = true;
	
}
TArray<AActor*> ATopDownHUD::GetSelectedActors()
{
	return SelectedActors;
}
