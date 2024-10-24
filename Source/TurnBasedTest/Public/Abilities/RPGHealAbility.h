// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RPGBaseAbility.h"
#include "RPGHealAbility.generated.h"

/**
 * 
 */
UCLASS()
class TURNBASEDTEST_API URPGHealAbility : public URPGBaseAbility
{
	GENERATED_BODY()
	
public:

	virtual void PerformAction(class ARPGBaseUnit* owner, class ARPGBaseUnit* target) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Heal;
};
