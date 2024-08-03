// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/AwakenThor.h"

#include "Kratos.h"
#include "CSW/AwakenThorFSM.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AAwakenThor::AAwakenThor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Fsm = CreateDefaultSubobject<UAwakenThorFSM>(TEXT("FSM"));

	Mjolnir = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mjolnir"));
	Mjolnir->SetupAttachment(GetMesh());

	MjolnirMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MjolnirMoveComp"));
	MjolnirMoveComp->SetUpdatedComponent(Mjolnir);
}

// Called when the game starts or when spawned
void AAwakenThor::BeginPlay()
{
	Super::BeginPlay();
	Mjolnir->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LeftHandSocket"));

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

	FTimerHandle tmpHandle;
	FTimerDelegate tmpDel;
	tmpDel.BindUFunction(this, FName("SetThorLocation"), NewLoc);
	GetWorld()->GetTimerManager().SetTimer(tmpHandle, tmpDel, 1.f, false);
}

void AAwakenThor::ThrowForTeleport(FVector Direction)
{
	Mjolnir->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mjolnir->SetRelativeRotation(Direction.Rotation() - FRotator(-180, 0, 0));
	Direction *= 2000;
	MjolnirMoveComp->Velocity = Direction;
	
}

void AAwakenThor::Teleport(FVector Target)
{
	SetActorLocation(Target);
	Mjolnir->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("LeftHandSocket"));
	MjolnirMoveComp->Velocity = FVector::ZeroVector;
}

UAwakenThorFSM* AAwakenThor::getFSM() const
{
	return Fsm;
}

void AAwakenThor::SetThorLocation(FVector NewLoc)
{
	SetActorLocation(NewLoc);
	MjolnirMoveComp->Velocity = FVector::ZeroVector;
	Mjolnir->SetRelativeLocation(FVector(0, 60, 20));
}

