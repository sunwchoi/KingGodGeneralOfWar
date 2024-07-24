// Fill out your copyright notice in the Description page of Project Settings.


#include "BDThor.h"
#include "BDThorFSM.h"

// Sets default values
ABDThor::ABDThor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스켈레탈 메시 데이터 로드

	//BDThorFSM 컴포넌트 추가
	fsm = CreateDefaultSubobject<UBDThorFSM>(TEXT("FSM"));
}

// Called when the game starts or when spawned
void ABDThor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABDThor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABDThor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

