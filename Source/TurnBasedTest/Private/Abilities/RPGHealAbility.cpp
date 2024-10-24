// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RPGHealAbility.h"

#include "RPGBaseUnit.h"

void URPGHealAbility::PerformAction(ARPGBaseUnit* owner, ARPGBaseUnit* target)
{
	target->TakeHealth(Heal);

	GEngine->AddOnScreenDebugMessage(-1, 300.f, FColor::Red, TEXT("Performing heal ability"));
}
