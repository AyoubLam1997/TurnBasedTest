// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Camera/CameraActor.h"

#include "RPGCombatSystem.generated.h"

UENUM(Blueprintable)
enum EBattleState
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
class TURNBASEDTEST_API ARPGCombatSystem : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class ARPGBaseUnit*> AllUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<class ARPGBaseUnit>> PlayerUnits;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TSubclassOf<class ARPGBaseUnit>> EnemyUnits;

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

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EBattleState> BattleState;
};
