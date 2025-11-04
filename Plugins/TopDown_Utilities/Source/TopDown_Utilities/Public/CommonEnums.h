
#pragma once
UENUM(BlueprintType)
enum class EResourceTypes : uint8
{
	Wood UMETA(DisplayName = "Wood"),
	Food UMETA(DisplayName = "Food"),
	Gold UMETA(DisplayName = "Gold"),
	Stone UMETA(DisplayName = "Stone"),
	Population UMETA(DisplayName = "Population")
};


UENUM(BlueprintType)
enum class EUnitRole : uint8
{
	Melee  UMETA(DisplayName = "Villager"),
	Ranged UMETA(DisplayName = "Archer"),
	Support UMETA(DisplayName = "Support")
};