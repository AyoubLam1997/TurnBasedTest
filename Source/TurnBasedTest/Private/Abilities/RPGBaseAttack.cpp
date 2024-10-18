// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RPGBaseAttack.h"

#include "RPGBaseUnit.h"

void URPGBaseAttack::PerformAction(ARPGBaseUnit* owner, ARPGBaseUnit* target)
{
	int baseAttack = Damage * (owner->Stats.Strength / 2);

	target->TakeDamage(baseAttack, ElementType);

	GEngine->AddOnScreenDebugMessage(-1, 300.f, FColor::Red, TEXT("Performing attack"));
}
