// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBaseUnit.h"

#include "Abilities/RPGBaseAbility.h"
#include "Net/UnrealNetwork.h"

FUnitStats::FUnitStats()
{
	CurrentHealth = Health;
}

void FUnitStats::RemoveHealth(int health)
{
	CurrentHealth -= health;
}

// Sets default values
ARPGBaseUnit::ARPGBaseUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));

	this->SetRootComponent(SkeletalMesh);
}

// Called when the game starts or when spawned
void ARPGBaseUnit::BeginPlay()
{
	Super::BeginPlay();

	LocationToMoveTowards = FVector(0, 0, 0);
}

void ARPGBaseUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPGBaseUnit, UnitState);
}

// Called every frame
void ARPGBaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch(UnitState)
	{
	case EUnitState::Idle:
		break;
	case EUnitState::MovingToLocation:
	{
		MoveToLocation(LocationToMoveTowards);

		double dis = FVector::Dist(GetActorLocation(), LocationToMoveTowards);

		if (dis <= 5)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Point reached"));
			SkeletalMesh->PlayAnimation(Attack, 0);
			UnitState = EUnitState::PerformingAbility;
		}
	}
	break;

	case EUnitState::PerformingAbility:

		if(!SkeletalMesh->IsPlaying())
		{
			SkeletalMesh->PlayAnimation(Dash, 0);
			UnitState = EUnitState::MovingBack;
		}

		break;

	case EUnitState::MovingBack:
		MoveToLocation(BattleLocation);
		break;
	}
}

// Called to bind functionality to input
void ARPGBaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Implementation is based on this source: https://rpg.fandom.com/wiki/Damage_Formula
void ARPGBaseUnit::TakeDamage(int damage, EUnitElementType attackerType)
{
	float damageMultiplier = ElementTypeChart::TypeDamage(attackerType, Type);

	int def = 100 + Stats.Defence;

	float def2 = 100.f / def;

	int dmg = damage * def2;

	dmg *= damageMultiplier;

	// TODO: Apply damage to unit
	GEngine->AddOnScreenDebugMessage(-1, 300.f, FColor::Red, FString::FromInt(dmg));
}

void ARPGBaseUnit::SetAbilityToPerform(URPGBaseAbility* ability)
{
	AbilityToPerform = ability;
}

void ARPGBaseUnit::MoveToLocation(FVector location)
{
	FVector value = FMath::Lerp(GetActorLocation(), location, .05f);

	SetActorLocation(value);
}

void ARPGBaseUnit::SetUnitBattleLocation(FVector location)
{
	BattleLocation = location;
}

void ARPGBaseUnit::SetLocationToMove(FVector location)
{
	LocationToMoveTowards = location;
}

void ARPGBaseUnit::SetUnitState(EUnitState state)
{
	UnitState = state;

	switch (UnitState)
	{
	case EUnitState::Idle:
		break;
	case EUnitState::MovingToLocation:
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Dashing"));

		SkeletalMesh->PlayAnimation(Dash, 0);
	}
	break;

	case EUnitState::PerformingAbility:

		SkeletalMesh->PlayAnimation(Attack, 0);

		break;

	case EUnitState::MovingBack:
		SkeletalMesh->PlayAnimation(Dash, 0);
		break;
	}
}

void ARPGBaseUnit::OnRep_OnStateSwitch()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Unit state is ") + FString::FromInt(UnitState));
}

void ARPGBaseUnit::OnRep_OnLocationSetip()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Unit location set"));
}

const bool ARPGBaseUnit::IsOnBattleLocation() const
{
	return (GetActorLocation() == BattleLocation);
}
