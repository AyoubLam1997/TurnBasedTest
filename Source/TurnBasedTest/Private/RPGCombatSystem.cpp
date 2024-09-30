// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCombatSystem.h"

#include "RPGBaseUnit.h"

#include "Abilities/RPGBaseAttack.h"

#include "GameState/GameStateRPG.h"

#include "Camera/CameraComponent.h"

void ARPGCombatSystem::BeginPlay()
{
	FVector pos1 = FVector(0, 200, 0);
	FVector pos2 = FVector(0, -200, 0);
	FRotator rot1 = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	//PlayerUnits.Add(rpg);

	for (int i = 0; i < PlayersToSpawn.Num(); i++)
	{
		float x = 200.f;

		float posX = -200.f + (x * i);

		ARPGBaseUnit* unit = GetWorld()->SpawnActor<ARPGBaseUnit>(PlayersToSpawn[i], FVector(posX, 400, 0), rot1, SpawnInfo);

		unit->SetUnitBattleLocation(FVector(posX, 400, 0));

		PlayerUnits.Add(unit);
		AllUnits.Add(unit);
	}
	for (int i = 0; i < EnemiesToSpawn.Num(); i++)
	{
		float x = 200.f;

		float posX = -200.f + (x * i);

		ARPGBaseUnit* unit = GetWorld()->SpawnActor<ARPGBaseUnit>(EnemiesToSpawn[i], FVector(posX, -400, 0), rot1, SpawnInfo);

		unit->SetUnitBattleLocation(FVector(posX, -400, 0));

		EnemyUnits.Add(unit);
		AllUnits.Add(unit);
	}

	ReturnRPGGameState()->AllUnits = AllUnits;;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Reordering based on speed"));

	SetUnits();

	CurrentRound = 1;

	BattleState = EBattleState::Setup;

	Super::BeginPlay();
}

void ARPGCombatSystem::Tick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Active Unit: ") + AllUnits[CurrentUnitIndex]->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Round: ") + FString::FromInt(CurrentRound));

	switch (BattleState)
	{
	case EBattleState::Setup:
		break;

	case EBattleState::PlayerChoosing:
		break;

	case EBattleState::UnitPerformingAction:

		double dis = FVector::Dist(AllUnits[CurrentUnitIndex]->GetActorLocation(), AllUnits[CurrentUnitIndex]->BattleLocation);

		if(AllUnits[CurrentUnitIndex]->UnitState == EUnitState::MovingBack && dis <= 5)
		{
			AllUnits[CurrentUnitIndex]->UnitState = EUnitState::Idle;

			CurrentUnitIndex += 1;

			if (CurrentUnitIndex >= AllUnits.Num())
			{
				CurrentUnitIndex = 0;

				CurrentRound += 1;
			}

			ReturnRPGGameState()->SetIndex(CurrentUnitIndex);

			BattleState = EBattleState::Setup;
		}

		break;

	}

	ReturnRPGGameState()->SetState(BattleState);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::SpaceBar))
	{
		CurrentUnitIndex += 1;

		if (CurrentUnitIndex >= AllUnits.Num())
		{
			CurrentUnitIndex = 0;

			CurrentRound += 1;
		}

		ReturnRPGGameState()->SetIndex(CurrentUnitIndex);

		/*if (BattleState == EBattleState::Setup)
			BattleState = EBattleState::SetNewRound;
		else
			BattleState = EBattleState::Setup;*/

		/*AllUnits[CurrentUnitIndex]->BaseAttack->GetDefaultObject<URPGBaseAttack>()->PerformAction(AllUnits[CurrentUnitIndex], AllUnits[0]);

		AllUnits[0]->SkeletalMesh->PlayAnimation(AllUnits[0]->Dash, 0);*/
	}

	//FVector value = FMath::Lerp(AllUnits[0]->GetActorLocation(), AllUnits[CurrentUnitIndex]->GetActorLocation(), .05f);

	//AllUnits[0]->SetActorLocation(value);

	/*ReturnRPGGameState()->BattleState = BattleState;
	ReturnRPGGameState()->CurrentUnitIndex = CurrentUnitIndex;
	ReturnRPGGameState()->CurrentRound = CurrentRound;*/

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
	//AllUnits[CurrentUnitIndex]->BaseAttack->GetDefaultObject<URPGBaseAttack>()->PerformAction(AllUnits[CurrentUnitIndex], EnemyUnits[index]->GetDefaultObject<ARPGBaseUnit>());

	if (PlayerUnits.Contains(AllUnits[CurrentUnitIndex]))
	{
		BattleState = EBattleState::UnitPerformingAction;

		ReturnRPGGameState()->SetState(BattleState);

		AllUnits[CurrentUnitIndex]->SkeletalMesh->PlayAnimation(AllUnits[CurrentUnitIndex]->Dash, 0);
		AllUnits[CurrentUnitIndex]->SetLocationToMove(EnemyUnits[index]->GetActorLocation());
		AllUnits[CurrentUnitIndex]->SetUnitState(EUnitState::MovingToLocation);
	}
	else if(EnemyUnits.Contains(AllUnits[CurrentUnitIndex]))
	{
		BattleState = EBattleState::UnitPerformingAction;

		ReturnRPGGameState()->SetState(BattleState);

		AllUnits[CurrentUnitIndex]->SkeletalMesh->PlayAnimation(AllUnits[CurrentUnitIndex]->Dash, 0);
		AllUnits[CurrentUnitIndex]->SetLocationToMove(PlayerUnits[index]->GetActorLocation());
		AllUnits[CurrentUnitIndex]->SetUnitState(EUnitState::MovingToLocation);
	}
}

void ARPGCombatSystem::PostLogin(APlayerController* NewPlayer)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Player has joined the session"));

	Super::PostLogin(NewPlayer);

	PlayerControllerList.Add(NewPlayer);
}

AGameStateRPG* ARPGCombatSystem::ReturnRPGGameState()
{
	return Cast<AGameStateRPG>(GameState);
}
