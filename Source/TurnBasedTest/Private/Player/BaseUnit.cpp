// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BaseUnit.h"
#include "AbilitySystemComponent.h"
#include "DefaultAttributeSet.h"

// Sets default values
ABaseUnit::ABaseUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void ABaseUnit::BeginPlay()
{
	Super::BeginPlay();
	

	if(IsValid(AbilitySystemComponent))
	{
		DefaultAttributeSet = AbilitySystemComponent->GetSet<UDefaultAttributeSet>();
	}
}

// Called every frame
void ABaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

