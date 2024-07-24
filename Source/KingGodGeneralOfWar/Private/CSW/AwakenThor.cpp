// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/AwakenThor.h"

#include "CSW/AwakenThorFSM.h"

// Sets default values
AAwakenThor::AAwakenThor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Fsm = CreateDefaultSubobject<UAwakenThorFSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void AAwakenThor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAwakenThor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAwakenThor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

