// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "CLMBasePawn.h"

ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = true;
}



void ATopDownPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TopDownHUD = Cast<ATopDownHUD>(GetHUD());
}

void ATopDownPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!DefaultInpuMappingContext)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(DefaultInpuMappingContext, 0);
		UE_LOG(LogTemp, Display, TEXT("Input mapping context added."));
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		//Bind Move function to move input action 
		//EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::Select);
		//EnhancedInputComponent->BindAction(CommandAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::Command);

		EnhancedInputComponent->BindAction(CommandAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::CommandActors);


		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &ATopDownPlayerController::SelectStart);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::SelectGoing);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Completed, this, &ATopDownPlayerController::SelectEnd);
	}
}

void ATopDownPlayerController::Command(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Trying to command...."));
	if (SelectedActor)
	{

		UE_LOG(LogTemp, Display, TEXT("Command Action"));
		if (SelectedActor->GetClass()->ImplementsInterface(UNavigableInterface::StaticClass()))
		{
			FHitResult HitResult;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);
			if(HitResult.bBlockingHit)
				INavigableInterface::Execute_CommandActor(SelectedActor, HitResult.Location);
		}
	}
	
}

void ATopDownPlayerController::CommandActors(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Trying to command multiple...."));
	if (!SelectedActors.IsEmpty())
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);
		if (HitResult.bBlockingHit) {
			for (AActor* a : SelectedActors)
			{
				if (a)
				{
					if (a->GetClass()->ImplementsInterface(UNavigableInterface::StaticClass()))
					{


						INavigableInterface::Execute_CommandActor(a, HitResult.Location);
					}
				}
			}
		}

		
	}

}

void ATopDownPlayerController::SelectStart(const FInputActionValue& Value)
{
	float MouseX, MouseY;
	GetMousePosition(MouseX, MouseY);
	SelectStartPos = FVector2D(MouseX, MouseY);
	//UE_LOG(LogTemp, Display, TEXT("Select Start"));
}

void ATopDownPlayerController::SelectGoing(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Display, TEXT("Select Going"));
	float MouseX, MouseY;
	GetMousePosition(MouseX, MouseY);
	SelectSize = FVector2D(MouseX-SelectStartPos.X, MouseY-SelectStartPos.Y);
	if(TopDownHUD)
		TopDownHUD->ShowSelection(SelectStartPos, SelectSize);
}

void ATopDownPlayerController::SelectEnd(const FInputActionValue& Value)
{
	if (TopDownHUD)
	{
		TopDownHUD->HideSelection();

		for (AActor* a : SelectedActors)
		{
			if (a)
			{
				if (a->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
				{
					ISelectableInterface::Execute_SelectActor(a, false);
				}
			}
		}
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ATopDownPlayerController::SelectMultiple, 0.1f,false);

		
		
	}
	
}
void ATopDownPlayerController::SelectMultiple()
{
	if (TopDownHUD)
	{
		auto AllSelectedActors = TopDownHUD->GetSelectedActors();
		TArray<AActor*> NavigableActors;
		SelectedActors.Empty();
		NavigableActors.Empty();
		for (AActor* a : AllSelectedActors)
		{
			if (a)
			{
				if (a->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
				{
					//UE_LOG(LogTemp, Display, TEXT("Select Actor in loop"));
					
					SelectedActors.AddUnique(a);
					if (a->GetClass()->ImplementsInterface(UNavigableInterface::StaticClass()))
						NavigableActors.AddUnique(a);
				}
			}
		}
		if (!NavigableActors.IsEmpty())
		{
			SelectedActors = NavigableActors;
		}
		
		for (AActor* a : SelectedActors)
			ISelectableInterface::Execute_SelectActor(a, true);
	}
}

void ATopDownPlayerController::Select(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Select Action"));

	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);

	// Deselect previously selected actor
	if (SelectedActor) 
	{
		if (SelectedActor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
		{
			ISelectableInterface::Execute_SelectActor(SelectedActor, false);
		}
	}
	
	SelectedActor = HitResult.GetActor();

	if (SelectedActor)
	{
		UE_LOG(LogTemp, Display, TEXT("Selected Actor: %s"), *SelectedActor->GetName());

		// Select new actor	
		if (SelectedActor->GetClass()->ImplementsInterface(USelectableInterface::StaticClass()))
		{
			ISelectableInterface::Execute_SelectActor(SelectedActor, true);
		}

	}
}
