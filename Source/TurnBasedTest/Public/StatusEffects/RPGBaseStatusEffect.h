// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPGBaseStatusEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TURNBASEDTEST_API URPGBaseStatusEffect : public UObject
{
	GENERATED_BODY()

public:
	
	virtual void HandleStatusEffect(class ARPGBaseUnit* target);
	
};
