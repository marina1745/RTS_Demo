// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnums.h"
#include "PawnData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPawnData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnInfo")
    EUnitRole unitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnInfo")
    FString DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnInfo")
    UTexture2D* Texture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PawnInfo")
    int32 DefaultHP;

    FPawnData() : unitType(EUnitRole::Melee), DisplayName(TEXT("Villager")), Texture(nullptr), DefaultHP(100) {}

    FPawnData(EUnitRole Type, FString Name, UTexture2D* Tex, int32 Default)
        : unitType(Type), DisplayName(Name), Texture(Tex), DefaultHP(Default) {
    }
};