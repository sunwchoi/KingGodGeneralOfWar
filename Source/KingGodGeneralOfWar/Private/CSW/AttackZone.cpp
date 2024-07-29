// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/AttackZone.h"

// Sets default values
AAttackZone::AAttackZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);
}

// Called when the game starts or when spawned
void AAttackZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAttackZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AAttackZone:: OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("123"));
}

