// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/GameStateRPG.h"
#include "RPGCombatSystem.h"
#include "Net/UnrealNetwork.h"

void AGameStateRPG::BeginPlay()
{

}

void AGameStateRPG::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, TEXT("Called"));

	DOREPLIFETIME_CONDITION(AGameStateRPG, BattleState, COND_None);
	DOREPLIFETIME(AGameStateRPG, CurrentUnitIndex);
}

void AGameStateRPG::SetState(EBattleState state)
{
	BattleState = state;
}

void AGameStateRPG::SetIndex(int index)
{
	CurrentUnitIndex = index;
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

