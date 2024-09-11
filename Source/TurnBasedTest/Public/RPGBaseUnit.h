// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RPGBaseUnit.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FUnitStats
{
	GENERATED_BODY()

public:

	FUnitStats() {};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Health;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Defence;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Speed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Strength;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Magic;
};

UCLASS()
class TURNBASEDTEST_API ARPGBaseUnit : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARPGBaseUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FUnitStats Stats;
};
