// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBaseUnit.h"

// Sets default values
ARPGBaseUnit::ARPGBaseUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

