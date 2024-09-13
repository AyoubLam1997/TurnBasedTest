// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RPGBaseUnit.generated.h"

UENUM(BlueprintType)
// Element types for units & attacks
enum EUnitElementType
{
	Normal = 0,
	Bludgeon,
	Slash,
	Grapple,
	Water,
	Fire
	//Wind
};

// Simulates basic strength/weakness chart, where damage is either doubled or halved
class ElementTypeChart
{
public:

	static float** Charts()
	{
		float** chart = new float*[6];

		/// Damage multipliers based on damage types
		/// DEV NOTE: These multipliers are for testing purposes. Daage should either be multiplied by 2 or .5
		///                      NORM   BLDG   SLSH   GRPL   WTR   FIRE
		chart[0] = new float[6] {1.f,   1.f,   1.f,   1.f,   1.f,  1.f};
		chart[1] = new float[6] {11.f,  12.f,  13.f,  14.f,  15.f, 16.f};
		chart[2] = new float[6] {.5f,   .5f,   .5f,   .5f,   .5f,  .5f};
		chart[3] = new float[6] {2.f,   2.f,   2.f,   2.f,   2.f,  2.f};
		chart[4] = new float[6] {25.f,  25.f,  25.f,  25.f,  25.f, 25.f};
		chart[5] = new float[6] {1.f,   2.f,   3.f,   4.f,   5.f,  6.f};

		return chart;
	}

	// Returns the multiplier based on attacking type & defender type
	static float TypeDamage(EUnitElementType attack, EUnitElementType defense)
	{
		// Cast element type to int to determine the column & row
		int column = static_cast<int>(attack);
		int row = static_cast<int>(defense);

		// Return chart value based on column & row
		return Charts()[column][row];
	}
};

USTRUCT(Blueprintable, BlueprintType)
struct FUnitStats
{
	GENERATED_BODY()

public:

	FUnitStats();

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

	int CurrentHealth;

	void RemoveHealth(int value);
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EUnitElementType> Type;

	void TakeDamage(int damage, EUnitElementType attackerType);
};
