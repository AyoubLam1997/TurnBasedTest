// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCombatSystem.h"

#include "RPGBaseUnit.h"

#include "Camera/CameraComponent.h"

void ARPGCombatSystem::BeginPlay()
{
	FVector pos1 = FVector(0, 200, 0);
	FVector pos2 = FVector(0, -200, 0);
	FRotator rot1 = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	//PlayerUnits.Add(rpg);

	for (int i = 0; i < PlayerUnits.Num(); i++)
	{
		ARPGBaseUnit* unit = GetWorld()->SpawnActor<ARPGBaseUnit>(PlayerUnits[i], pos1, rot1, SpawnInfo);

		AllUnits.Add(unit);
	}
	for (int i = 0; i < EnemyUnits.Num(); i++)
	{
		ARPGBaseUnit* unit = GetWorld()->SpawnActor<ARPGBaseUnit>(EnemyUnits[i], pos2, rot1, SpawnInfo);

		AllUnits.Add(unit);
	}
	for(int i = 0; i < AllUnits.Num(); i++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(AllUnits[i]->Stats.Speed));
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Reordering based on speed"));

	SetUnits();

	for (int i = 0; i < AllUnits.Num(); i++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(AllUnits[i]->Stats.Speed));
	}

	CurrentRound = 1;

	Super::BeginPlay();

}

void ARPGCombatSystem::Tick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Active Unit: ") + AllUnits[CurrentUnitIndex]->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Round: ") + FString::FromInt(CurrentRound));

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
	{
		CurrentUnitIndex += 1;

		if (CurrentUnitIndex >= AllUnits.Num())
		{
			CurrentUnitIndex = 0;

			CurrentRound += 1;
		}
	}

	Super::Tick(DeltaTime);
}

void ARPGCombatSystem::SetUnits()
{
	// Set player & enemy units on startup

	OrderUnitsBySpeed();
}

void ARPGCombatSystem::OrderUnitsBySpeed()
{
	AllUnits.Sort([](const ARPGBaseUnit& U1, const ARPGBaseUnit& U2)
		{
			return U1.Stats.Speed > U2.Stats.Speed;
		});
}
