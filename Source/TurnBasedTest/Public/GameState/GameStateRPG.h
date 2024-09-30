// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStateRPG.generated.h"

/**
 * 
 */

enum class EBattleState;

UCLASS()
class TURNBASEDTEST_API AGameStateRPG : public AGameState
{
	GENERATED_BODY()
	
public:

	UPROPERTY(ReplicatedUsing = OnRep_OnBattleStateSwitch)
	TEnumAsByte<EBattleState> BattleState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class ARPGBaseUnit*> AllUnits;

	UPROPERTY(Replicated)
	int CurrentUnitIndex;

	UPROPERTY(ReplicatedUsing = OnRep_OnRoundSwitch)
	int CurrentRound;

	void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;

	void SetState(EBattleState state);

	void SetIndex(int index);

	UFUNCTION()
	void OnRep_OnBattleStateSwitch();
	UFUNCTION()
	void OnRep_OnUnitSwitch();
	UFUNCTION()
	void OnRep_OnUnitIndex();
	UFUNCTION()
	void OnRep_OnRoundSwitch();
};
