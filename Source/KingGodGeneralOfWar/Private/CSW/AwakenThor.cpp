// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/AwakenThor.h"

#include "CSW/AwakenThorFSM.h"
#include "CSW/PoundThunderAttackZone.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/UnitConversion.h"

// Sets default values
AAwakenThor::AAwakenThor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Fsm = CreateDefaultSubobject<UAwakenThorFSM>(TEXT("FSM"));

	Mjolnir = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mjolnir"));
	Mjolnir->SetupAttachment(RootComponent);

	MjolnirMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MjolnirMoveComp"));
	MjolnirMoveComp->SetUpdatedComponent(Mjolnir);
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

UProjectileMovementComponent* AAwakenThor::GetMjolnirMoveComp() const
{
	return MjolnirMoveComp;
}

UStaticMeshComponent* AAwakenThor::GetMjolnir() const
{
	return Mjolnir;
}

void AAwakenThor::Move(FVector NewLoc)
{
	FVector dir = NewLoc - GetActorLocation();
	MjolnirMoveComp->Velocity = dir;

	FTimerHandle tmpHandle;
	FTimerDelegate tmpDel;
	tmpDel.BindUFunction(this, FName("SetThorLocation"), NewLoc);
	GetWorld()->GetTimerManager().SetTimer(tmpHandle, tmpDel, 1.f, false);
}

void AAwakenThor::PoundThunderAttack(const FTransform& Target) const
{
	GetWorld()->SpawnActor<APoundThunderAttackZone>(PoundThunderAttackZoneBP, Target);
}

void AAwakenThor::SetThorLocation(FVector NewLoc)
{
	SetActorLocation(NewLoc);
	MjolnirMoveComp->Velocity = FVector::ZeroVector;
	Mjolnir->SetRelativeLocation(FVector(0, 60, 20));
}

