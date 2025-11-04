// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownPlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "CLMBasePawn.h"
#include <NavigationSystem.h>

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
/*
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

*/

void ATopDownPlayerController::CommandActors(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Trying to command multiple...."));
	if (!SelectedActors.IsEmpty())
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, HitResult);
		if (HitResult.bBlockingHit) {
			TArray<ACLMBasePawn*> Melee, Ranged, Others;
			for (AActor* a : SelectedActors)
			{
				if (a)
				{
					if (a->GetClass()->ImplementsInterface(UNavigableInterface::StaticClass()))
					{
						ACLMBasePawn* P = Cast < ACLMBasePawn>(a);
						switch (P->UnitRole)
						{
						case EUnitRole::Melee:  Melee.Add(P);  break;
						case EUnitRole::Ranged: Ranged.Add(P); break;
						default:                Others.Add(P); break;
						}


						//INavigableInterface::Execute_CommandActor(a, HitResult.Location);
					}
				}
			}
			FVector nextRow = FVector::Zero();

			
			ArrangeUnits(Melee, EUnitRole::Melee, HitResult.Location, nextRow);
			ArrangeUnits(Ranged, EUnitRole::Ranged, nextRow, nextRow);
			ArrangeUnits(Others, EUnitRole::Support, nextRow, nextRow);


		}

		
	}

}

void ATopDownPlayerController::ArrangeUnits(TArray<ACLMBasePawn*> Units, EUnitRole Type, FVector StartPos, FVector& EndPos)
{
	Units.Sort([StartPos](const ACLMBasePawn& A, const ACLMBasePawn& B)
		{
			return FVector::DistSquared(A.GetActorLocation(), StartPos) <
				FVector::DistSquared(B.GetActorLocation(), StartPos);
		});
	
	int32 UnitsPerRow = 0;
	float Spacing = 100.f;         
	GetRoleAttributes(Type, UnitsPerRow, Spacing);
	UnitsPerRow = FMath::Max(1, UnitsPerRow);

	const float RowSpacing = Spacing; 

	if (Units.Num() == 0)
	{
		EndPos = StartPos;
		return;
	}

	// 3) Compute formation basis vectors in the XY plane
	//    Forward points from the squad centroid toward StartPos
	FVector Centroid = FVector::ZeroVector;
	for (ACLMBasePawn* U : Units) { if (U) Centroid += U->GetActorLocation(); }
	Centroid /= float(Units.Num());

	FVector Forward = (StartPos - Centroid);
	Forward.Z = 0.f; // flatten so rows don’t tilt with terrain bumps
	if (!Forward.Normalize()) { Forward = FVector::ForwardVector; }

	FVector Right = FVector::CrossProduct(Forward, FVector::UpVector);
	if (!Right.Normalize()) { Right = FVector::RightVector; }

	
	TArray<float> LateralSlots;
	LateralSlots.Reserve(UnitsPerRow);

	if ((UnitsPerRow % 2) == 1)
	{
		// Odd: 0, -1, +1, -2, +2, ...
		LateralSlots.Add(0.f);
		for (int s = 1; s <= UnitsPerRow / 2; ++s)
		{
			LateralSlots.Add(-float(s));
			if ((int)LateralSlots.Num() < UnitsPerRow) LateralSlots.Add(+float(s));
		}
	}
	else
	{
		// Even: -0.5, +0.5, -1.5, +1.5, ...
		for (int s = 0; s < UnitsPerRow / 2; ++s)
		{
			LateralSlots.Add(-(0.5f + s));
			if ((int)LateralSlots.Num() < UnitsPerRow) LateralSlots.Add(+(0.5f + s));
		}
	}

	// 5) Place units row by row
	EndPos = StartPos;
	for (int32 i = 0; i < Units.Num(); ++i)
	{
		ACLMBasePawn* U = Units[i];
		if (!U) continue;

		const int32 RowIdx = i / UnitsPerRow;
		const int32 ColIdx = i % UnitsPerRow;

		const FVector RowOrigin = StartPos - Forward * (RowIdx * RowSpacing);
		const FVector TargetPos = RowOrigin + Right * (LateralSlots[ColIdx] * Spacing);

		INavigableInterface::Execute_CommandActor(U, TargetPos);
		
	}
	//Potential Next Row Start
	EndPos = StartPos - Forward * (((Units.Num() - 1) / UnitsPerRow + 1) * RowSpacing);
}



/*




/*
void ATopDownPlayerController::CommandActors(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Trying to command multiple...."));
	if (SelectedActors.IsEmpty()) return;

	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Camera, false, Hit);
	if (!Hit.bBlockingHit) return;

	const FVector Target = Hit.Location;

	// Compute a sensible facing: from group center toward click
	FVector groupCenter = FVector::ZeroVector;
	int32 count = 0;

	TArray<ACLMBasePawn*> Melee, Ranged, Others;
	for (AActor* A : SelectedActors)
	{
		if (!A) continue;
		ACLMBasePawn* P = Cast<ACLMBasePawn>(A);
		if (!P) continue;

		groupCenter += P->GetActorLocation();
		count++;

		switch (P->UnitRole)
		{
		case EUnitRole::Melee:  Melee.Add(P);  break;
		case EUnitRole::Ranged: Ranged.Add(P); break;
		default:                Others.Add(P); break;
		}
	}
	if (count == 0) return;
	groupCenter /= (float)count;

	FVector Forward = (Target - groupCenter);
	Forward.Z = 0.f;
	Forward = Forward.GetSafeNormal();
	if (Forward.IsNearlyZero()) Forward = FVector::ForwardVector;

	// Parameterization — can be exposed as UPROPERTY later if you want
	auto makeAndDispatch = [&](const TArray<ACLMBasePawn*>& Units, float baseOffsetRows, bool front)
		{
			if (Units.Num() == 0) return;

			// Use the first unit’s spacing as baseline, but you could compute min/max etc.
			float spacing = Units[0]->FormationSpacing;
			int32 perRow = Units[0]->UnitsPerRow;

			// Front rows: negative RowOffsetMultiplier to push toward the click (front).
			// Back rows: positive to push away (behind).
			float rowMul = front ? -1.f : +1.f;

			// Start melee a bit *before* the click so they form a front line closer to target.
			FVector origin = Target + (front ? -Forward : Forward) * (baseOffsetRows * spacing);

			TArray<FVector> slots;
			BuildRowedSlots(slots, Units.Num(), origin, Forward, spacing, perRow, rowMul);

			// Optionally: “grow” spacing by capsule radii to avoid overlap
			for (int32 i = 0; i < Units.Num(); ++i)
			{
				FVector navPoint = slots[i];
				// Nudge by capsule radius if you want extra clearance:
				// float r = Units[i]->GetCapsuleRadius();
				// navPoint += Forward * (front ? -r*0.25f : r*0.25f);

				// Snap to navmesh
				ProjectToNav(GetWorld(), navPoint, navPoint);

				INavigableInterface::Execute_CommandActor(Units[i], navPoint);
			}
		};

	
}
*/

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
		OnActorsSelected.Broadcast(SelectedActors);
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
