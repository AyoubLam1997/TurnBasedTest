// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBaseUnit.h"

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
	
}

// Called every frame
void ARPGBaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

FUnitStats::FUnitStats()
{
	CurrentHealth = Health;
}

void FUnitStats::RemoveHealth(int health)
{
	CurrentHealth -= health;
}
