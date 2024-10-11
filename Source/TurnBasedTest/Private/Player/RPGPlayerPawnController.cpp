// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RPGPlayerPawnController.h"
#include "Net/UnrealNetwork.h"

// Called when the game starts or when spawned
void ARPGPlayerPawnController::BeginPlay()
{
	Super::BeginPlay();
}

void ARPGPlayerPawnController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARPGPlayerPawnController, Team);
}

// Called every frame
void ARPGPlayerPawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARPGPlayerPawnController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
