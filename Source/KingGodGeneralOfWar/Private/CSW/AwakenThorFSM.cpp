// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/AwakenThorFSM.h"

#include "CSW/AwakenThor.h"
#include "CSW/AwakenThorAnim.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
	Anim = Cast<UAwakenThorAnim>(Me->GetMesh()->GetAnimInstance());
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

void UAwakenThorFSM::SetState(EAwakenThorState NewState)
{
	State = NewState;
}

void UAwakenThorFSM::IdleState()
{
	// UE_LOG(LogTemp, Warning, TEXT("Idle"));
	
	// FVector targetLoc = Target->GetActorLocation();
	// FVector myLoc = Me->GetActorLocation();
	// FRotator rot = (targetLoc - myLoc).Rotation();
	//
	// Me->SetActorRotation(FRotator(0, rot.Yaw, 0));
	// if (FVector::Dist(targetLoc, myLoc) > 1050)
	// 	SetState(EAwakenThorState::Move);
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = Me->GetActorLocation();
	FRotator rot = (targetLoc - myLoc).Rotation();
	
	Me->SetActorRotation(FRotator(0, rot.Yaw, 0));
	
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	if (CurrentTime > IdleDelayTime)
	{
		State = EAwakenThorState::Move;
		Anim->SetState(EAwakenThorState::Move);
		CurrentTime = 0;
	}
}


void UAwakenThorFSM::MoveState()
{
}

void UAwakenThorFSM::AttackState()
{
	bPlay = true;
	UE_LOG(LogTemp, Warning, TEXT("Attack"));

	Me->PoundThunderAttack(Target->GetTransform());
	Anim->PlayPoundAttackAnim(FName("Attack0"));
}

void UAwakenThorFSM::DamageState()
{
}

void UAwakenThorFSM::DieState()
{
}

void UAwakenThorFSM::ThrowForTeleport()
{
	auto* camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector target = camera->GetCameraLocation() + camera->GetActorForwardVector() * 800;

	Me->ThrowForTeleport(target);
}

void UAwakenThorFSM::Teleport()
{
	auto* camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	FVector target = camera->GetCameraLocation() + camera->GetActorForwardVector() * 800;
	
	Me->Teleport(target);
}

