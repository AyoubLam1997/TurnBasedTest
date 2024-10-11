// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/GameStateRPG.h"
#include "RPGCombatSystem.h"
#include "RPGBaseUnit.h"
#include "Abilities/RPGBaseAttack.h"

#include "Net/UnrealNetwork.h"

void AGameStateRPG::BeginPlay()
{
	//FVector pos1 = FVector(0, 200, 0);
	//FVector pos2 = FVector(0, -200, 0);
	//FRotator rot1 = FRotator(0, 0, 0);
	//FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	////PlayerUnits.Add(rpg);

	//for (int i = 0; i < PlayersToSpawn.Num(); i++)
	//{
	//	float x = 200.f;

	//	float posX = -200.f + (x * i);

	//	ARPGBaseUnit* unit = GetWorld()->SpawnActor<ARPGBaseUnit>(PlayersToSpawn[i], FVector(posX, 400, 0), rot1, SpawnInfo);

	//	unit->SetUnitBattleLocation(FVector(posX, 400, 0));

	//	PlayerUnits.Add(unit);
	//	AllUnits.Add(unit);
	//}
	//for (int i = 0; i < EnemiesToSpawn.Num(); i++)
	//{
	//	float x = 200.f;

	//	float posX = -200.f + (x * i);

	//	ARPGBaseUnit* unit = GetWorld()->SpawnActor<ARPGBaseUnit>(EnemiesToSpawn[i], FVector(posX, -400, 0), rot1, SpawnInfo);

	//	unit->SetUnitBattleLocation(FVector(posX, -400, 0));

	//	EnemyUnits.Add(unit);
	//	AllUnits.Add(unit);
	//}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Reordering based on speed"));

	//SetUnits();

	//CurrentRound = 1;

	//BattleState = EBattleState::Setup;

	PrimaryActorTick.bCanEverTick = true;

	Super::BeginPlay();
}

void AGameStateRPG::Tick(float DeltaTime)
{
	CurrentRound = 0;

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Unit: ") + AllUnits[CurrentUnitIndex]->GetName());
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Round: ") + CurrentRound);

	switch (BattleState)
	{
	case EBattleState::Setup:
		break;

	case EBattleState::PlayerChoosing:
		break;

	case EBattleState::UnitPerformingAction:

		double dis = FVector::Dist(AllUnits[CurrentUnitIndex]->GetActorLocation(), AllUnits[CurrentUnitIndex]->BattleLocation);

		if (AllUnits[CurrentUnitIndex]->UnitState == EUnitState::MovingBack && dis <= 5)
		{
			AllUnits[CurrentUnitIndex]->UnitState = EUnitState::Idle;

			CurrentUnitIndex += 1;

			if (CurrentUnitIndex >= AllUnits.Num())
			{
				CurrentUnitIndex = 0;

				CurrentRound += 1;
			}

			if (PlayerUnits.Contains(AllUnits[CurrentUnitIndex]))
				BattleState = EBattleState::PlayerChoosing;
			else if (EnemyUnits.Contains(AllUnits[CurrentUnitIndex]))
				BattleState = EBattleState::EnemyChoosing;

			//BattleState = EBattleState::Setup;
		}

		break;

	}

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

void AGameStateRPG::SetUnits()
{
	// Set player & enemy units on startup

	OrderUnitsBySpeed();
}

// Order units based on speed. The higher the speed stat, the higher the turn order
void AGameStateRPG::OrderUnitsBySpeed()
{
	AllUnits.Sort([](const ARPGBaseUnit& U1, const ARPGBaseUnit& U2)
		{
			return U1.Stats.Speed > U2.Stats.Speed;
		});
}

void AGameStateRPG::PerformUnitAttack()
{
	AllUnits[CurrentUnitIndex]->BaseAttack->GetDefaultObject<URPGBaseAttack>()->PerformAction(AllUnits[CurrentUnitIndex], AllUnits[0]);

	AllUnits[0]->SkeletalMesh->PlayAnimation(AllUnits[0]->Dash, 0);
}

void AGameStateRPG::PerformUnitAttackOnSpecifiedTarget(int index)
{
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Unit attacks: ") + AllUnits[index]->GetName());

	if (PlayerUnits.Contains(AllUnits[CurrentUnitIndex]))
	{
		BattleState = EBattleState::UnitPerformingAction;

		AllUnits[CurrentUnitIndex]->SkeletalMesh->PlayAnimation(AllUnits[CurrentUnitIndex]->Dash, 0);
		AllUnits[CurrentUnitIndex]->SetLocationToMove(EnemyUnits[index]->GetActorLocation());
		AllUnits[CurrentUnitIndex]->SetUnitState(EUnitState::MovingToLocation);
	}
	else if (EnemyUnits.Contains(AllUnits[CurrentUnitIndex]))
	{
		BattleState = EBattleState::UnitPerformingAction;

		AllUnits[CurrentUnitIndex]->SkeletalMesh->PlayAnimation(AllUnits[CurrentUnitIndex]->Dash, 0);
		AllUnits[CurrentUnitIndex]->SetLocationToMove(PlayerUnits[index]->GetActorLocation());
		AllUnits[CurrentUnitIndex]->SetUnitState(EUnitState::MovingToLocation);
	}
}

void AGameStateRPG::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameStateRPG, P1Pawn);
	DOREPLIFETIME(AGameStateRPG, P2Pawn);
	DOREPLIFETIME(AGameStateRPG, BattleState);
	DOREPLIFETIME(AGameStateRPG, AllUnits);
	DOREPLIFETIME(AGameStateRPG, PlayerUnits);
	DOREPLIFETIME(AGameStateRPG, EnemyUnits);
	DOREPLIFETIME(AGameStateRPG, CurrentUnitIndex);
	DOREPLIFETIME(AGameStateRPG, CurrentRound);
}

void AGameStateRPG::SetState(EBattleState state)
{
	BattleState = state;
}

void AGameStateRPG::SetIndex(int index)
{
	CurrentUnitIndex = index;
}

void AGameStateRPG::SetRound(int index)
{
	CurrentRound = index;
}

void AGameStateRPG::OnRep_OnBattleStateSwitch()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Battle state has been switched"));

	if (BattleState == EBattleState::UnitPerformingAction)
	{

	}
}

void AGameStateRPG::OnRep_OnUnitSwitch()
{

}

void AGameStateRPG::OnRep_OnUnitIndex()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Unit has been switched"));
}

void AGameStateRPG::OnRep_OnRoundSwitch()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Round has switched"));
}

//void PerformUnitAttackOnSpecifiedTarget_Implementation(int index)
//{
//
//}
