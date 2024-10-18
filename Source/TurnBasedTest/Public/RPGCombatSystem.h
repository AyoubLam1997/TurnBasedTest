// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"

#include "Camera/CameraActor.h"

#include "RPGCombatSystem.generated.h"

class AGameStateRPG;

UENUM(Blueprintable)
enum class EBattleState
{
	Setup = 0,
	PlayerChoosing,
	EnemyChoosing,
	UnitPerformingAction,
	SetNewRound,
	FinishingCombat
};

/**
 * 
 */
UCLASS()
class TURNBASEDTEST_API ARPGCombatSystem : public AGameMode
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class ARPGBaseUnit*> AllUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class ARPGBaseUnit*> PlayerUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class ARPGBaseUnit*> EnemyUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<class ARPGBaseUnit>> PlayersToSpawn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<class ARPGBaseUnit>> EnemiesToSpawn;

	//TArray<TSubclassOf<class ARPGBaseUnit>> EnemyUnits;

	class ARPGBaseUnit* CurrentActiveUnit;

	int CurrentUnitIndex;

	int CurrentRound;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void SetUnits();
	void OrderUnitsBySpeed();

	UFUNCTION(BlueprintCallable)
	void PerformUnitAttack();
	UFUNCTION(BlueprintCallable)
	void PerformUnitAttackOnSpecifiedTarget(int index);
	UFUNCTION(BlueprintCallable)
	void SetChosenAbilityToUnit(TSubclassOf<class URPGBaseAbility> ability);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<APlayerController*> PlayerControllerList;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	AGameStateRPG* ReturnRPGGameState();

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EBattleState> BattleState;
};
