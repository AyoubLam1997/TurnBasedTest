// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/GameStateRPG.h"
#include "RPGCombatSystem.h"
#include "RPGBaseUnit.h"
#include "Abilities/RPGBaseAbility.h"
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
	case EBattleState::EnemyChoosing:
		break;

	case EBattleState::UnitPerformingAction:

		double dis = FVector::Dist(AllUnits[CurrentUnitIndex]->GetActorLocation(), AllUnits[CurrentUnitIndex]->BattleLocation);

		if (AllUnits[CurrentUnitIndex]->UnitState == EUnitState::PerformingAbility && !AllUnits[CurrentUnitIndex]->SkeletalMesh->IsPlaying())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, TEXT("Calling function for test"));
			//AllUnits[CurrentUnitIndex]->PerformAbility();

			//AllUnits[CurrentUnitIndex]->AbilityToPerform->GetDefaultObject<URPGBaseAbility>()->PerformAction(AllUnits[CurrentUnitIndex], AllUnits[CurrentUnitIndex]->AbilityToPerform->GetDefaultObject<URPGBaseAbility>()->Target);

			AbilityToPerform->PerformAction(AbilityCaller, AbilityTarget);
		}

		if (AllUnits[CurrentUnitIndex]->UnitState == EUnitState::MovingBack && dis <= 5)
		{
			AllUnits[CurrentUnitIndex]->UnitState = EUnitState::Idle;

			/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, FString::FromInt(AllUnits[CurrentUnitIndex]->Stats.CurrentHealth));
			AllUnits[CurrentUnitIndex]->TakeDamage(20, EUnitElementType::Normal);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::FromInt(AllUnits[CurrentUnitIndex]->Stats.CurrentHealth));*/

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
	if (AllUnits[CurrentUnitIndex]->AbilityToPerform != nullptr)
	{
		switch (BattleState)
		{
		case EBattleState::PlayerChoosing:
			break;
		case EBattleState::EnemyChoosing:
			break;
		}

		if (PlayerUnits.Contains(AllUnits[CurrentUnitIndex]))
		{
			AllUnits[CurrentUnitIndex]->AbilityToPerform->GetDefaultObject<URPGBaseAbility>()->Target = EnemyUnits[index];

			URPGBaseAbility* abl = AllUnits[CurrentUnitIndex]->AbilityToPerform->GetDefaultObject<URPGBaseAbility>();

			//AllUnits[CurrentUnitIndex]->OnAbility.AddUFunction(abl, "PerformAction");

			if (abl->AbilityType == EAbilityTargetType::Enemy)
			{
				//abl->PerformAction(AllUnits[CurrentUnitIndex], EnemyUnits[index]);

				BattleState = EBattleState::UnitPerformingAction;

				AllUnits[CurrentUnitIndex]->SkeletalMesh->PlayAnimation(AllUnits[CurrentUnitIndex]->Dash, 0);
				AllUnits[CurrentUnitIndex]->SetLocationToMove(EnemyUnits[index]->GetActorLocation());
				AllUnits[CurrentUnitIndex]->SetUnitState(EUnitState::MovingToLocation);

				AbilityToPerform = AllUnits[CurrentUnitIndex]->AbilityToPerform->GetDefaultObject<URPGBaseAbility>();
				AbilityCaller = AllUnits[CurrentUnitIndex];
				AbilityTarget = EnemyUnits[index];

				/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, FString::FromInt(EnemyUnits[index]->Stats.CurrentHealth));
				EnemyUnits[index]->TakeDamage(20, EUnitElementType::Normal);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::FromInt(EnemyUnits[index]->Stats.CurrentHealth));*/
			}
			else if(abl->AbilityType == EAbilityTargetType::Team)
			{
				BattleState = EBattleState::UnitPerformingAction;

				AllUnits[CurrentUnitIndex]->SkeletalMesh->PlayAnimation(AllUnits[CurrentUnitIndex]->Dash, 0);
				AllUnits[CurrentUnitIndex]->SetLocationToMove(PlayerUnits[index]->GetActorLocation());
				AllUnits[CurrentUnitIndex]->SetUnitState(EUnitState::MovingToLocation);

				AbilityToPerform = AllUnits[CurrentUnitIndex]->AbilityToPerform->GetDefaultObject<URPGBaseAbility>();
				AbilityCaller = AllUnits[CurrentUnitIndex];
				AbilityTarget = PlayerUnits[index];
			}
		}
		else if (EnemyUnits.Contains(AllUnits[CurrentUnitIndex]))
		{
			AllUnits[CurrentUnitIndex]->AbilityToPerform->GetDefaultObject<URPGBaseAbility>()->Target = PlayerUnits[index];

			URPGBaseAbility* abl = AllUnits[CurrentUnitIndex]->AbilityToPerform->GetDefaultObject<URPGBaseAbility>();

			//AllUnits[CurrentUnitIndex]->OnAbility.AddUFunction(abl, "PerformAction");

			BattleState = EBattleState::UnitPerformingAction;

			if (abl->AbilityType == EAbilityTargetType::Team)
			{
				BattleState = EBattleState::UnitPerformingAction;

				AllUnits[CurrentUnitIndex]->SkeletalMesh->PlayAnimation(AllUnits[CurrentUnitIndex]->Dash, 0);
				AllUnits[CurrentUnitIndex]->SetLocationToMove(EnemyUnits[index]->GetActorLocation());
				AllUnits[CurrentUnitIndex]->SetUnitState(EUnitState::MovingToLocation);

				AbilityToPerform = AllUnits[CurrentUnitIndex]->AbilityToPerform->GetDefaultObject<URPGBaseAbility>();
				AbilityCaller = AllUnits[CurrentUnitIndex];
				AbilityTarget = EnemyUnits[index];
			}
			else if (abl->AbilityType == EAbilityTargetType::Enemy)
			{
				//abl->PerformAction(AllUnits[CurrentUnitIndex], PlayerUnits[index]);

				BattleState = EBattleState::UnitPerformingAction;

				AllUnits[CurrentUnitIndex]->SkeletalMesh->PlayAnimation(AllUnits[CurrentUnitIndex]->Dash, 0);
				AllUnits[CurrentUnitIndex]->SetLocationToMove(PlayerUnits[index]->GetActorLocation());
				AllUnits[CurrentUnitIndex]->SetUnitState(EUnitState::MovingToLocation);

				AbilityToPerform = AllUnits[CurrentUnitIndex]->AbilityToPerform->GetDefaultObject<URPGBaseAbility>();
				AbilityCaller = AllUnits[CurrentUnitIndex];
				AbilityTarget = PlayerUnits[index];
				/*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, FString::FromInt(PlayerUnits[index]->Stats.CurrentHealth));
				PlayerUnits[index]->TakeDamage(20, EUnitElementType::Normal);

				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::FromInt(PlayerUnits[index]->Stats.CurrentHealth));*/
			}
		}
	}
}

void AGameStateRPG::SetChosenAbilityToUnit(TSubclassOf<URPGBaseAbility> ability)
{
	AllUnits[CurrentUnitIndex]->AbilityToPerform = ability;
}

void AGameStateRPG::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameStateRPG, AbilityToPerform);
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
