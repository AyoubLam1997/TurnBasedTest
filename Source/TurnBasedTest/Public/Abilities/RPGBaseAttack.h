// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/RPGBaseAbility.h"
#include "RPGBaseAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, DefaultToInstanced)
class TURNBASEDTEST_API URPGBaseAttack : public URPGBaseAbility
{
	GENERATED_BODY()

public:

	virtual void PerformAction(class ARPGBaseUnit* owner, class ARPGBaseUnit* target) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<enum EUnitElementType> Type;
};
