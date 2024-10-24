// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RPGBaseAbility.h"

#include "RPGBaseUnit.h"


void URPGBaseAbility::PerformAction(ARPGBaseUnit* owner, ARPGBaseUnit* target)
{
	GEngine->AddOnScreenDebugMessage(-1, 300.f, FColor::Blue, TEXT("Performing ability"));
}
