// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPGBaseAbility.generated.h"

UENUM(BlueprintType)
// Element types for units & attacks
enum EAbilityTargetType
{
	Self = 0,
	Team,
	Enemy,
	All
};

UCLASS(Blueprintable, DefaultToInstanced)
class TURNBASEDTEST_API URPGBaseAbility : public UObject
{
	GENERATED_BODY()

public:

	class ARPGBaseUnit* Target;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString AbilityName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EAbilityTargetType> AbilityType;

	UFUNCTION()
	virtual void PerformAction(class ARPGBaseUnit* owner, class ARPGBaseUnit* target);
	
};
