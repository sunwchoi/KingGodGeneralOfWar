// Fill out your copyright notice in the Description page of Project Settings.


#include "Kratos.h"

// Sets default values
AKratos::AKratos()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKratos::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKratos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKratos::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKratos::Damage(int DamageValue, EAttackType AttackType)
{

}

