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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	TEnumAsByte<EBattleState> BattleState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	TArray<class ARPGBaseUnit*> AllUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	TArray<class ARPGBaseUnit*> PlayerUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	TArray<class ARPGBaseUnit*> EnemyUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)

	class ARPGBaseUnit* CurrentActiveUnit;

	UPROPERTY(BlueprintReadWrite, Replicated)
	class  ARPGPlayerPawnController* P1Pawn;
	UPROPERTY(BlueprintReadWrite, Replicated)
	class  ARPGPlayerPawnController* P2Pawn;

	UPROPERTY(BlueprintReadWrite, Replicated)
	int CurrentUnitIndex = 0;

	UPROPERTY(BlueprintReadOnly, Replicated)
	int CurrentRound = 0;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void SetUnits();
	void OrderUnitsBySpeed();

	void SetState(EBattleState state);
	void SetIndex(int index);
	UFUNCTION(BlueprintCallable)
	void SetRound(int index);

	UFUNCTION(BlueprintCallable)
	void PerformUnitAttack();
	UFUNCTION(BlueprintCallable/*, NetMulticast, Reliable*/)
	void PerformUnitAttackOnSpecifiedTarget(int index);
	UFUNCTION(BlueprintCallable)
	void SetChosenAbilityToUnit(TSubclassOf<class URPGBaseAbility> ability);

	UPROPERTY(BlueprintReadWrite, Replicated)
	class URPGBaseAbility* AbilityToPerform;
	UPROPERTY(BlueprintReadWrite, Replicated)
	class ARPGBaseUnit* AbilityCaller;
	UPROPERTY(BlueprintReadWrite, Replicated)
	class ARPGBaseUnit* AbilityTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<APlayerController*> PlayerControllerList;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void OnRep_OnBattleStateSwitch();
	UFUNCTION()
	void OnRep_OnUnitSwitch();
	UFUNCTION()
	void OnRep_OnUnitIndex();
	UFUNCTION()
	void OnRep_OnRoundSwitch();
};
