// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include <TopDownHUD.h>
#include "TopDownPlayerController.generated.h"


/**
 * 
 */

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ACLMBasePawn;

UCLASS()
class TOPDOWN_UTILITIES_API ATopDownPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	ATopDownPlayerController();

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultInpuMappingContext;


private:
	/** Select Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SelectAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CommandAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ATopDownHUD> TopDownHUD;




	UPROPERTY()
	AActor* SelectedActor;
	TArray<AActor*> SelectedActors;
	FVector2D SelectStartPos;
	FVector2D SelectSize;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void Select(const FInputActionValue& Value);
	void Command(const FInputActionValue& Value);

	void SelectStart(const FInputActionValue& Value);
	void SelectGoing(const FInputActionValue& Value);
	void SelectEnd(const FInputActionValue& Value);

	void SelectMultiple();
	void CommandActors(const FInputActionValue& Value);
};
