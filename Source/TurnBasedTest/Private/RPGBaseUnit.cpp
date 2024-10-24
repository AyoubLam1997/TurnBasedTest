// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBaseUnit.h"

#include "Abilities/RPGBaseAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

FUnitStats::FUnitStats()
{
	CurrentHealth = Health;
}

void FUnitStats::RemoveHealth(int value)
{
	CurrentHealth = CurrentHealth - value;
}

void FUnitStats::AddHealth(int value)
{
	CurrentHealth += value;

	if (CurrentHealth > Health)
		CurrentHealth = Health;
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

	//AbilityToPerform->AddToRoot();

	LocationToMoveTowards = FVector(0, 0, 0);
}

void ARPGBaseUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPGBaseUnit, Stats);
	DOREPLIFETIME(ARPGBaseUnit, UnitState);
	DOREPLIFETIME(ARPGBaseUnit, SkeletalMesh);
	DOREPLIFETIME(ARPGBaseUnit, BattleLocation);
	DOREPLIFETIME(ARPGBaseUnit, LocationToMoveTowards);
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
			SetUnitState(EUnitState::PerformingAbility);
		}
	}
	break;

	case EUnitState::PerformingAbility:

		if(!SkeletalMesh->IsPlaying())
		{
			//OnAbility.Broadcast(this, AbilityToPerform->GetDefaultObject<URPGBaseAbility>()->Target);
			//OnAbility.Clear();

			//AbilityToPerform->GetDefaultObject<URPGBaseAbility>()->PerformAction(this, AbilityToPerform->GetDefaultObject<URPGBaseAbility>()->Target);

			SetUnitState(EUnitState::MovingBack);
		}

		break;

	case EUnitState::MovingBack:
		MoveToLocation(BattleLocation);
		break;
	}
}

// Called to bind functionality to input
void ARPGBaseUnit::PerformAbility()
{
	AbilityToPerform->GetDefaultObject<URPGBaseAbility>()->PerformAction(this, AbilityToPerform->GetDefaultObject<URPGBaseAbility>()->Target);
}

// Called to bind functionality to input
void ARPGBaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Implementation is based on this source: https://rpg.fandom.com/wiki/Damage_Formula
void ARPGBaseUnit::TakeDamage(int damage, TEnumAsByte<EUnitElementType> attackerType)
{
	float damageMultiplier = ElementTypeChart::TypeDamage(attackerType, Type);

	int def = 100 + Stats.Defence;

	float def2 = 100.f / def;

	int dmg = damage * def2;

	dmg *= damageMultiplier;

	Stats.RemoveHealth(dmg);
}

// Implementation is based on this source: https://rpg.fandom.com/wiki/Damage_Formula
void ARPGBaseUnit::TakeUnscaledDamage(int damage)
{
	Stats.RemoveHealth(damage);
}

void ARPGBaseUnit::TakeHealth(int health)
{
	Stats.AddHealth(health);
}

void ARPGBaseUnit::SetAbilityToPerform(TSubclassOf<URPGBaseAbility> ability)
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

const bool ARPGBaseUnit::IsDead() const
{
	if (Stats.CurrentHealth <= 0.f)
		return 1;

	return 0;
}

const bool ARPGBaseUnit::IsOnBattleLocation() const
{
	return (GetActorLocation() == BattleLocation);
}
