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

	FString state = UEnum::GetValueAsString(State);
	UE_LOG(LogTemp, Warning, TEXT("state: %s\n"), *state);
	// FString anim = UEnum::GetValueAsString(Anim->GetState());
	
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
	case EAwakenThorState::KickAttack:
		KickAttackState();
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

		if (dist <= 200.f)
		{
			NextStates.Add(EAwakenThorState::MeleeAttackChange);
		}
		else if (200.f < dist && dist <= 700.f)
		{
			NextStates.Add(EAwakenThorState::RangedAttackChange);
			NextStates.Add(EAwakenThorState::BackTeleport);
			NextStates.Add(EAwakenThorState::BackTeleport);
			NextStates.Add(EAwakenThorState::LeftTeleport);
			NextStates.Add(EAwakenThorState::RightTeleport);
		}
		else
		{
			NextStates.Add(EAwakenThorState::RangedAttackChange);
			NextStates.Add(EAwakenThorState::Dash);
			NextStates.Add(EAwakenThorState::Dash);
			NextStates.Add(EAwakenThorState::Dash);
			NextStates.Add(EAwakenThorState::Dash);
		}
		
		CurrentTime = 0.f;
		int32 idx = FMath::RandRange(0, NextStates.Num() - 1);
		// State = NextStates[idx];
		State = EAwakenThorState::MeleeAttackChange;
		/*if (State != EAwakenThorState::Dash)
		{
			if (State == EAwakenThorState::LeftTeleport || State == EAwakenThorState::RightTeleport || State == EAwakenThorState::BackTeleport)
			{
				Anim->SetState(EAwakenThorState::Teleport);
				CurrentTime = IdleDelayTime - 0.3f;
			}
			else 
				Anim->SetState(State);
		}*/
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
	Anim->PlayTeleportMontage();
}

void UAwakenThorFSM::MeleeAttackChangeState()
{
	TArray<EAwakenThorState> AttackStates = {
		EAwakenThorState::ClapAttack,
		EAwakenThorState::KickAttack,
	};

	int32 idx = FMath::RandRange(0, AttackStates.Num() - 1);
	
	State = AttackStates[idx];
}

void UAwakenThorFSM::RangedAttackChangeState()
{
	TArray<EAwakenThorState> AttackStates = {
		EAwakenThorState::PoundAttack,
		EAwakenThorState::JumpAttack,
	};

	int32 idx = FMath::RandRange(0, AttackStates.Num() - 1);
	
	State = AttackStates[idx];
}

void UAwakenThorFSM::JumpAttackState()
{
}


void UAwakenThorFSM::PoundAttackState()
{
	Anim->PlayPoundAttackMontage();
}

void UAwakenThorFSM::KickAttackState()
{
	Anim->PlayKickAttackMontage();
}

void UAwakenThorFSM::ClapAttackState()
{
	Anim->PlayClapAttackMontage();
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
	float minDx = 300.f;
	float maxDx = 1000.f;
	float minDy = -1000.f;
	float maxDy = 1000.f;
	
	AttackZone.Empty();
	AttackZone.Add(std::make_pair(FVector(targetLoc.X, targetLoc.Y, 0), PoundZoneRadius));

	float dx = FMath::RandRange(minDx, maxDx);
	float dy = FMath::RandRange(minDy, maxDy);

	AttackZone.Add(std::make_pair(FVector(targetLoc.X + dx, targetLoc.Y + dy, 0), PoundZoneRadius));

	dx = FMath::RandRange(minDx, maxDx);
	dy = FMath::RandRange(minDy, maxDy);
	AttackZone.Add(std::make_pair(FVector(targetLoc.X - dx, targetLoc.Y + dy, 0), PoundZoneRadius));
	
	DrawAttackZoneDecal();
}

void UAwakenThorFSM::StartPoundAttack()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), Me->PoundCameraShake, Me->GetActorLocation(), 0, 15000);
	SphereOverlapForPound(AttackZone.Pop(), 10, EHitType::STUN, false);
}

void UAwakenThorFSM::StartClapAttack()
{
	FVector attackLoc = Me->GetMesh()->GetBoneLocation(FName("LeftHand"));

	AttackZone.Empty();
	AttackZone.Add(std::make_pair(attackLoc, ClapZoneRadius));
	SphereOverlap(10, EHitType::NB_HIGH, true);
}

void UAwakenThorFSM::StartKickAttack()
{
	FVector attackLoc = Me->GetMesh()->GetBoneLocation(FName("RightFoot"));
	
	AttackZone.Empty();
	AttackZone.Add(std::make_pair(attackLoc, KickZoneRadius));
	SphereOverlap(10, EHitType::NB_HIGH, true);
}

void UAwakenThorFSM::ReadyJumpAttack()
{
	FVector org = Me->GetActorLocation();
	FVector fwd = Me->GetActorForwardVector();
	org.Z = 0;

	AttackZone.Empty();
	for (int i = 0; i < 5; i++)
		AttackZone.Add(std::make_pair(org + (fwd + FRotator(0, 75 * i, 0).Vector()) * 1000, JumpAtkZoneRaidus));

	DrawAttackZoneDecal();
}

void UAwakenThorFSM::StartJumpAttack()
{
	SphereOverlap(10, EHitType::STUN, false);
	FVector loc = Me->GetActorLocation();
	FVector target = Target->GetActorLocation();
	
	Me->SetActorLocation(FVector(target.X, target.Y, loc.Z) + Target->GetActorForwardVector() * 100);
	FVector newLoc = Me->GetActorLocation();
	newLoc.Z = 0;
	AttackZone.Empty();
	AttackZone.Add(std::make_pair(newLoc, JumpAtkZoneRaidus));

	DrawAttackZoneDecal();
}

void UAwakenThorFSM::StartFallAttack()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), Me->PoundCameraShake, Me->GetActorLocation(), 0, 15000);
	SphereOverlap(20, EHitType::NB_HIGH, false);
}

void UAwakenThorFSM::GetHitDirectionString(EAttackDirectionType AtkDir, FString& Str)
{
	Str = UEnum::GetValueAsString(AtkDir).Mid(22);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Str);
}

void UAwakenThorFSM::SetDamage(float Damage, EAttackDirectionType AtkDir)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage"));
	bool isDie = Me->SetHp(Damage);
	Me->UpdateHpUI();

	if (isDie)
	{
		State = EAwakenThorState::Die;
		Anim->PlayDieMontage();
	}
	FString Str;
	GetHitDirectionString(AtkDir, Str);
	State = EAwakenThorState::Damage;
	Anim->PlayHitMontage();
	Anim->JumpToHitSection(Str);
}

void UAwakenThorFSM::SetJump(bool Value)
{
	bJump = Value;
}

EAwakenThorState UAwakenThorFSM::GetState() const
{
	return State;
}

void UAwakenThorFSM::SphereOverlap(float Damage, EHitType HitType, bool IsMelee)
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
				Target->Damage(Me, Damage, HitType, IsMelee);
			} // SetDamage
		}

		FVector newLoc(zone.first.X, zone.first.Y, 20);
		if (!IsMelee)
			GetWorld()->SpawnActor<AActor>(Me->LightBPClass, newLoc, FRotator::ZeroRotator);
		// DrawDebugSphere(GetWorld(), zone.first, zone.second, 32, FColor::Green, false, 1.f);
	}
	if(!IsMelee)
	DrawAttackZoneDecal(true);
}

void UAwakenThorFSM::SphereOverlapForPound(const std::pair<FVector, float>& zone, float Damage, EHitType HitType, bool IsMelee)
{
	TArray<AActor *> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));

	
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
			Target->Damage(Me, Damage, HitType, IsMelee);
		} 
	}

	FVector newLoc(zone.first.X, zone.first.Y, 20);
	if (!IsMelee)
		GetWorld()->SpawnActor<AActor>(Me->LightBPClass, newLoc, FRotator::ZeroRotator);
	if(!IsMelee)
		DrawAttackZoneDecalForPound(zone, true);
}

void UAwakenThorFSM::DrawAttackZoneDecalForPound(const std::pair<FVector, float>& zone, bool isAttack)
{
	FVector size = FVector(zone.second, zone.second,0);

	UMaterialInterface* AttackZoneDecal;
	if (!isAttack)
	{
		AttackZoneDecal = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/CSW/Material/AttackZone1.AttackZone1'"));
		size.Z = 10;
	}
	else
	{
		AttackZoneDecal = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/CSW/Material/AttackZone.AttackZone'"));
		size.Z = 3;
	}
			
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), AttackZoneDecal, size, zone.first, FRotator::ZeroRotator, 2.f);
}

void UAwakenThorFSM::DrawAttackZoneDecal(bool isAttack)
{
	for (auto zone : AttackZone)
	{
		FVector size = FVector(zone.second, zone.second,0);

		UMaterialInterface* AttackZoneDecal;
		if (!isAttack)
		{
			AttackZoneDecal = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/CSW/Material/AttackZone1.AttackZone1'"));
			size.Z = 10;
		}
		else
		{
			AttackZoneDecal = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/CSW/Material/AttackZone.AttackZone'"));
			size.Z = 3;
		}
			
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), AttackZoneDecal, size, zone.first, FRotator::ZeroRotator, 2.5f);
	}
}

