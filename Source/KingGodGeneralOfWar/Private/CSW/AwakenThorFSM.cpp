// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/AwakenThorFSM.h"

#include "CSW/AwakenThor.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UAwakenThorFSM::UAwakenThorFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}



// Called when the game starts
void UAwakenThorFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ACharacter* tmp = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (tmp)
		Target = tmp;
	Me = Cast<AAwakenThor>(GetOwner());

	
}


// Called every frame
void UAwakenThorFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	switch (State)
	{
	case EAwakenThorState::Idle:
		IdleState();
		break;
	case EAwakenThorState::Move:
		MoveState();
		break;
	case EAwakenThorState::Attack:
		AttackState();
		break;
	case EAwakenThorState::Damage:
		DamageState();
		break;
	case EAwakenThorState::Die:
		DieState();
		break;
	}
}

void UAwakenThorFSM::IdleState()
{
	UE_LOG(LogTemp, Warning, TEXT("Idle"));
	
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = Me->GetActorLocation();
	FRotator rot = (targetLoc - myLoc).Rotation();

	Me->SetActorRotation(rot);
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > IdleDelayTime)
	{
		if (FVector::Dist(targetLoc, myLoc) > 1200)
			State = EAwakenThorState::Move;
		else
			State = EAwakenThorState::Attack;
		CurrentTime = 0;
	}
}

void UAwakenThorFSM::MoveState()
{
	UE_LOG(LogTemp, Warning, TEXT("Move"));

	FVector nextLoc = Target->GetControlRotation().Vector() * 500 + Target->GetActorLocation();
	FVector dir = nextLoc - Me->GetActorLocation();
	
	Me->SetActorLocation(Me->GetActorLocation() + dir.GetSafeNormal() * TeleportDist);
	
	State = EAwakenThorState::Idle;
}

void UAwakenThorFSM::AttackState()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack"));

	State = EAwakenThorState::Idle;
}

void UAwakenThorFSM::DamageState()
{
}

void UAwakenThorFSM::DieState()
{
}
