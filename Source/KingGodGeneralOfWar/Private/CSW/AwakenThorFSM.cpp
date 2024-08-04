// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/AwakenThorFSM.h"

#include "Kratos.h"
#include "CSW/AwakenThor.h"
#include "CSW/AwakenThorAnim.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	AKratos* tmp = Cast<AKratos>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (tmp)
		Target = tmp;
	Me = Cast<AAwakenThor>(GetOwner());
	Anim = Cast<UAwakenThorAnim>(Me->GetMesh()->GetAnimInstance());
}


// Called every frame
void UAwakenThorFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FString state = UEnum::GetValueAsString(State);
	// FString anim = UEnum::GetValueAsString(Anim->GetState());
	// UE_LOG(LogTemp, Warning, TEXT("state: %s, \n anim: %s"), *state, *anim);
	
	switch (State)
	{
	case EAwakenThorState::Idle:
		IdleState();
		break;
	case EAwakenThorState::Move:
		MoveState();
		break;
	case EAwakenThorState::Dash:
		DashState();
		break;
	case EAwakenThorState::Teleport:
		TeleportState();
		break;
	case EAwakenThorState::MeleeAttackChange:
		MeleeAttackChangeState();
		break;
	case EAwakenThorState::RangedAttackChange:
		RangedAttackChangeState();
		break;
	case EAwakenThorState::JumpAttack:
		JumpAttackState();
		break;
	case EAwakenThorState::PoundAttack:
		PoundAttackState();
		break;
	case EAwakenThorState::ClapAttack:
		ClapAttackState();
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
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = Me->GetActorLocation();
	FVector dir = targetLoc - myLoc;
	FRotator rot = dir.Rotation();
	
	Me->SetActorRotation(FRotator(0, rot.Yaw, 0));
	
	CurrentTime += GetWorld()->DeltaTimeSeconds;
	
	if (CurrentTime > IdleDelayTime)
	{
		float dist = FVector::Dist(myLoc, targetLoc);
		TArray<EAwakenThorState> NextStates;

		if (dist <= 500.f)
		{
			NextStates.Add(EAwakenThorState::MeleeAttackChange);
		}
		else if (500.f < dist && dist <= 1000.f)
		{
			NextStates.Add(EAwakenThorState::RangedAttackChange);
			NextStates.Add(EAwakenThorState::Dash);
		}
		else
		{
			NextStates.Add(EAwakenThorState::Dash);
			NextStates.Add(EAwakenThorState::Teleport);
		}
		
		int32 idx = FMath::RandRange(0, NextStates.Num() - 1);
		// State = NextStates[idx];
		State = EAwakenThorState::JumpAttack;
		if (State != EAwakenThorState::Dash)
		{
			if (State == EAwakenThorState::LeftTeleport || State == EAwakenThorState::RightTeleport || State == EAwakenThorState::BackTeleport)
				Anim->SetState(EAwakenThorState::Teleport);
			else 
				Anim->SetState(State);
		}
		CurrentTime = 0.f;
	}
}


void UAwakenThorFSM::MoveState()
{
}

void UAwakenThorFSM::DashState()
{
	FVector targetLoc = Target->GetActorLocation();
	FVector myLoc = Me->GetActorLocation();
	FVector dir = targetLoc - myLoc;
	FRotator rot = dir.Rotation();
	
	Me->SetActorRotation(FRotator(0, rot.Yaw, 0));
	float dist = FVector::Dist(Target->GetActorLocation(), Me->GetActorLocation());
	
	Me->AddMovementInput(dir);
	
	UE_LOG(LogTemp, Warning, TEXT("Dash"));
	if (dist < 200.f)
	{
		Me->GetCharacterMovement()->MaxWalkSpeed = 50.f;
		State = EAwakenThorState::MeleeAttackChange;
		Anim->SetState(EAwakenThorState::MeleeAttackChange);
	}
	else if (dist < 600.f)
	{
		Me->GetCharacterMovement()->MaxWalkSpeed = 3000.f;
	}
	else
	{
		if (Me->GetCharacterMovement()->MaxWalkSpeed < 500.f)
			Me->GetCharacterMovement()->MaxWalkSpeed += 10.f;
		else
			Me->GetCharacterMovement()->MaxWalkSpeed = 10000.f;
	}
}

void UAwakenThorFSM::TeleportState()
{
}

void UAwakenThorFSM::MeleeAttackChangeState()
{
	TArray<EAwakenThorState> AttackStates = {
		EAwakenThorState::ClapAttack,
		EAwakenThorState::KickAttack,
	};

	int32 idx = FMath::RandRange(0, AttackStates.Num() - 1);
	
	State = AttackStates[idx];
	Anim->SetState(State);
}

void UAwakenThorFSM::RangedAttackChangeState()
{
	TArray<EAwakenThorState> AttackStates = {
		EAwakenThorState::PoundAttack,
	};

	int32 idx = FMath::RandRange(0, AttackStates.Num() - 1);
	
	State = AttackStates[idx];
	Anim->SetState(State);
}

void UAwakenThorFSM::JumpAttackState()
{
}


void UAwakenThorFSM::PoundAttackState()
{
}

void UAwakenThorFSM::KickAttackState()
{
}

void UAwakenThorFSM::ClapAttackState()
{
}

void UAwakenThorFSM::DamageState()
{
}

void UAwakenThorFSM::DieState()
{
}

void UAwakenThorFSM::LookTeleportDirection()
{
	FVector dir;
	
	if (State == EAwakenThorState::LeftTeleport)
		dir = Me->GetActorRightVector() * -1;
	else if (State == EAwakenThorState::BackTeleport)
		dir = Me->GetActorForwardVector() * -1;
	else if (State == EAwakenThorState::RightTeleport)
		dir = Me->GetActorRightVector();
	else
		dir = Me->GetActorForwardVector();

	Me->SetActorRotation(dir.Rotation());
}

void UAwakenThorFSM::ThrowForTeleport()
{
	
	Me->ThrowForTeleport(Me->GetActorForwardVector());
}

void UAwakenThorFSM::Teleport()
{
	if (State == EAwakenThorState::Teleport)
		Me->Teleport(Target->GetActorLocation() + Target->GetActorForwardVector() * 200);
	else
		Me->Teleport(Me->GetActorLocation() + Me->GetActorForwardVector() * 1000);
}


void UAwakenThorFSM::ReadyPoundAttack()
{
	FVector targetLoc = Target->GetActorLocation();
	float zoneRad = 100.f;
	float minDx = 300.f;
	float maxDx = 1000.f;
	float minDy = -1000.f;
	float maxDy = 1000.f;
	
	AttackZone.Empty();
	AttackZone.Add(std::make_pair(FVector(targetLoc.X, targetLoc.Y, 0), zoneRad));

	float dx = FMath::RandRange(minDx, maxDx);
	float dy = FMath::RandRange(minDy, maxDy);

	AttackZone.Add(std::make_pair(FVector(targetLoc.X + dx, targetLoc.Y + dy, 0), zoneRad));

	dx = FMath::RandRange(minDx, maxDx);
	dy = FMath::RandRange(minDy, maxDy);
	AttackZone.Add(std::make_pair(FVector(targetLoc.X - dx, targetLoc.Y + dy, 0), zoneRad));
	
	for (auto zone : AttackZone)
	{
		FVector size = FVector(100, 100, 5);
	
		UMaterialInterface* AttackZoneDecal = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/CSW/Material/AttackZone.AttackZone'"));
		UE_LOG(LogTemp, Warning, TEXT("ThorPoundAttack"));
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), AttackZoneDecal, size, zone.first, FRotator::ZeroRotator, 5.f);
	}
}

void UAwakenThorFSM::StartPoundAttack()
{
	SphereOverlap(EHitType::STUN, false);
}

void UAwakenThorFSM::StartClapAttack()
{
	FVector attackLoc = Me->GetMesh()->GetBoneLocation(FName("LeftHand"));
	float zoneRadius = 50.f;

	AttackZone.Empty();
	AttackZone.Add(std::make_pair(attackLoc, zoneRadius));
	SphereOverlap(EHitType::NB_MEDIUM, true);
}

void UAwakenThorFSM::StartKickAttack()
{
	FVector attackLoc = Me->GetMesh()->GetBoneLocation(FName("RightFoot"));
	float zoneRadius = 50.f;

	AttackZone.Empty();
	AttackZone.Add(std::make_pair(attackLoc, zoneRadius));
	SphereOverlap(EHitType::NB_MEDIUM, true);
}

void UAwakenThorFSM::ReadyJumpAttack()
{
	FVector org = Me->GetActorLocation();
	FVector fwd = Me->GetActorForwardVector();
	float zoneRadius = 100.f;
	org.Z = 0;

	AttackZone.Empty();
	for (int i = 0; i < 5; i++)
		AttackZone.Add(std::make_pair(org + (fwd + FRotator(0, 75 * i, 0).Vector()) * 1000, zoneRadius));

	for (auto zone : AttackZone)
	{
		FVector size = FVector(100, 100, 5);
	
		UMaterialInterface* AttackZoneDecal = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/CSW/Material/AttackZone.AttackZone'"));
		UE_LOG(LogTemp, Warning, TEXT("ThorPoundAttack"));
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), AttackZoneDecal, size, zone.first, FRotator::ZeroRotator, 5.f);
	}
}

void UAwakenThorFSM::StartJumpAttack()
{
	SphereOverlap(EHitType::NB_MEDIUM, false);
	FVector loc = Me->GetActorLocation();
	FVector target = Target->GetActorLocation();
	float zoneRadius = 100.f;
	
	Me->SetActorLocation(FVector(target.X, target.Y, loc.Z) + Target->GetActorForwardVector() * 100);
	FVector newLoc = Me->GetActorLocation();\
	newLoc.Z = 0;
	AttackZone.Empty();
	AttackZone.Add(std::make_pair(newLoc, zoneRadius));

	for (auto zone : AttackZone)
	{
		FVector size = FVector(100, 100, 5);
	
		UMaterialInterface* AttackZoneDecal = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/CSW/Material/AttackZone.AttackZone'"));
		UE_LOG(LogTemp, Warning, TEXT("ThorPoundAttack"));
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), AttackZoneDecal, size, zone.first, FRotator::ZeroRotator, 5.f);
	}
}

void UAwakenThorFSM::StartFallAttack()
{
	SphereOverlap(EHitType::NB_MEDIUM, false);
}

void UAwakenThorFSM::GetHitDirectionString(EAttackDirectionType AtkDir, FString& Str)
{
	Str = UEnum::GetValueAsString(AtkDir).Mid(22);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Str);
}

void UAwakenThorFSM::SetDamage(float Damage, EAttackDirectionType AtkDir)
{
	if (State != EAwakenThorState::Idle)
		return ;
	FString Str;
	GetHitDirectionString(AtkDir, Str);
	State = EAwakenThorState::Damage;
	Anim->SetState(State);
	Anim->PlayHitMontage();
	Anim->JumpToHitSection(Str);
}

void UAwakenThorFSM::SetJump(bool Value)
{
	bJump = Value;
}

void UAwakenThorFSM::SphereOverlap(EHitType HitType, bool IsMelee)
{

	TArray<AActor *> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));

	for (auto zone : AttackZone)
	{	
		UKismetSystemLibrary::SphereOverlapActors(
			GetWorld(),
			zone.first,
			zone.second,
			ObjectTypes,
			AKratos::StaticClass(),
			TArray<AActor*>(),
			OverlappedActors
			);
		if (OverlappedActors.Num() > 0)
		{
			if(Cast<AKratos>(OverlappedActors.Top()))
			{
				FString tmp = UEnum::GetValueAsString(State);
				UE_LOG(LogTemp, Warning, TEXT("%s"), *tmp);
				Target->Damage(1, HitType, IsMelee);
			} // SetDamage
		}

		DrawDebugSphere(GetWorld(), zone.first, zone.second, 32, FColor::Green, false, 1.f);
	}
}

