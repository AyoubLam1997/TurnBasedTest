// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/SkeletalMeshComponent.h"

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
		/// DEV NOTE: These multipliers are for testing purposes. Damage should either be multiplied by 2 or .5
		///                      NORM   BLDG   SLSH   GRPL   WTR   FIRE
		chart[0] = new float[6] {1.f,   1.f,   1.f,   1.f,   1.f,  1.f}; // NORM
		chart[1] = new float[6] {1.f,   .5f,   2.F,   .5f,   1.f,  1.f}; // BLDG
		chart[2] = new float[6] {1.f,   2.f,   .5f,   .5f,   1.f,  1.f}; // SLSH
		chart[3] = new float[6] {1.f,   .5f,   2.f,   2.f,   1.f,  2.f}; // GRPL
		chart[4] = new float[6] {1.f,   1.f,   1.f,   1.F,   .5f,  2.f}; // WTR
		chart[5] = new float[6] {1.f,   1.f,   1.f,   1.f,   2.f,  .5f}; // FIRE

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//class URPGBaseAttack* BaseAttack;
	TSubclassOf<class URPGBaseAttack> BaseAttack;

	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* Attack;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* Dash;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	void TakeDamage(int damage, EUnitElementType attackerType);
};
