// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCombatSystem.h"

#include "RPGBaseUnit.h"

#include "Abilities/RPGBaseAttack.h"

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
		float x = 200.f;

		float posX = -200.f + (x * i);

		ARPGBaseUnit* unit = GetWorld()->SpawnActor<ARPGBaseUnit>(PlayerUnits[i], FVector(posX, 400, 0), rot1, SpawnInfo);

		AllUnits.Add(unit);
	}
	for (int i = 0; i < EnemyUnits.Num(); i++)
	{
		float x = 200.f;

		float posX = -200.f + (x * i);

		ARPGBaseUnit* unit = GetWorld()->SpawnActor<ARPGBaseUnit>(EnemyUnits[i], FVector(posX, -400, 0), rot1, SpawnInfo);

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

	BattleState = EBattleState::Setup;

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


		if (BattleState == EBattleState::Setup)
			BattleState = EBattleState::SetNewRound;
		else
			BattleState = EBattleState::Setup;

		/*AllUnits[CurrentUnitIndex]->BaseAttack->GetDefaultObject<URPGBaseAttack>()->PerformAction(AllUnits[CurrentUnitIndex], AllUnits[0]);

		AllUnits[0]->SkeletalMesh->PlayAnimation(AllUnits[0]->Dash, 0);*/
	}

	//FVector value = FMath::Lerp(AllUnits[0]->GetActorLocation(), AllUnits[CurrentUnitIndex]->GetActorLocation(), .05f);

	//AllUnits[0]->SetActorLocation(value);

	Super::Tick(DeltaTime);
}

void ARPGCombatSystem::SetUnits()
{
	// Set player & enemy units on startup

	OrderUnitsBySpeed();
}

// Order units based on speed. The higher the speed stat, the higher the turn order
void ARPGCombatSystem::OrderUnitsBySpeed()
{
	AllUnits.Sort([](const ARPGBaseUnit& U1, const ARPGBaseUnit& U2)
		{
			return U1.Stats.Speed > U2.Stats.Speed;
		});
}

void ARPGCombatSystem::PerformUnitAttack()
{
	AllUnits[CurrentUnitIndex]->BaseAttack->GetDefaultObject<URPGBaseAttack>()->PerformAction(AllUnits[CurrentUnitIndex], AllUnits[0]);

	AllUnits[0]->SkeletalMesh->PlayAnimation(AllUnits[0]->Dash, 0);
}

void ARPGCombatSystem::PerformUnitAttackOnSpecifiedTarget(int index)
{
	AllUnits[CurrentUnitIndex]->BaseAttack->GetDefaultObject<URPGBaseAttack>()->PerformAction(AllUnits[CurrentUnitIndex], EnemyUnits[index]->GetDefaultObject<ARPGBaseUnit>());

	AllUnits[CurrentUnitIndex]->SkeletalMesh->PlayAnimation(AllUnits[CurrentUnitIndex]->Dash, 0);
}
