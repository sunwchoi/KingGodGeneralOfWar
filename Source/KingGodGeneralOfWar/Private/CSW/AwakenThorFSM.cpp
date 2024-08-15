// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW/AwakenThorFSM.h"

#include "Kratos.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "CSW/AwakenThor.h"
#include "CSW/AwakenThorAnim.h"
#include "CSW/CSWGameMode.h"
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
	GameMode = Cast<ACSWGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
		GameMode->SetEnemyHpBar(1);
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
	case EAwakenThorState::Dash:
		DashState();
		break;
	case EAwakenThorState::BackTeleport:
	case EAwakenThorState::LeftTeleport:
	case EAwakenThorState::RightTeleport:
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
			// NextStates.Add(EAwakenThorState::BackTeleport);
			// NextStates.Add(EAwakenThorState::LeftTeleport);
			// NextStates.Add(EAwakenThorState::RightTeleport);
		}
		else if (200.f < dist && dist <= 600.f)
		{
			NextStates.Add(EAwakenThorState::RangedAttackChange);
			// NextStates.Add(EAwakenThorState::BackTeleport);
			// NextStates.Add(EAwakenThorState::LeftTeleport);
			// NextStates.Add(EAwakenThorState::RightTeleport);
		}
		else
		{
			NextStates.Add(EAwakenThorState::Teleport);
			NextStates.Add(EAwakenThorState::Dash);
		}
		
		CurrentTime = 0.f;
		int32 idx = FMath::RandRange(0, NextStates.Num() - 1);
		// State = NextStates[idx];
		State = EAwakenThorState::PoundAttack;
		if (State == EAwakenThorState::Dash || State == EAwakenThorState::LeftTeleport || State == EAwakenThorState::RightTeleport || State == EAwakenThorState::BackTeleport || State == EAwakenThorState::Teleport)
			bSuperArmor = true;
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
	
	if (dist < 200.f)
	{
		Me->GetCharacterMovement()->MaxWalkSpeed = 50.f;
		State = EAwakenThorState::MeleeAttackChange;
		Anim->SetState(EAwakenThorState::MeleeAttackChange);
	}
	else
	{
		if (Me->GetCharacterMovement()->MaxWalkSpeed < 500.f)
			Me->GetCharacterMovement()->MaxWalkSpeed += 10.f;
		else if (Me->GetCharacterMovement()->MaxWalkSpeed < 2000.f)
			Me->GetCharacterMovement()->MaxWalkSpeed += 400.f;
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
	Anim->PlayJumpAttackMontage();
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
	Anim->PlayDieMontage();
}

void UAwakenThorFSM::LookTeleportDirection()
{
	FVector dir;
	FVector fwd = Target->GetActorLocation() - Me->GetActorLocation();
	fwd.Normalize();
	
	if (State == EAwakenThorState::LeftTeleport)
		dir = FVector::CrossProduct(FVector::DownVector, fwd);
	else if (State == EAwakenThorState::BackTeleport)
		dir = fwd * -1;
	else if (State == EAwakenThorState::RightTeleport)
		dir = FVector::CrossProduct(FVector::UpVector, fwd);
	else
		dir = fwd;

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
		Me->Teleport(Me->GetActorLocation() + Me->GetActorForwardVector() * 700);
}


void UAwakenThorFSM::ReadyPoundAttack()
{
	FVector targetLoc = Target->GetActorLocation();

	float x, y, dx, dy;
	
	float minDx = 300.f;
	float maxDx = 1000.f;
	float minDy = -1000.f;
	float maxDy = 1000.f;
	
	AttackZone.Empty();
	x = targetLoc.X;
	y = targetLoc.Y;
	AttackZone.Add(std::make_pair(FVector(x, y, getFloorZ(FVector2D(x, y))), PoundZoneRadius));
	DecalZone.Add(DrawAttackZoneDecal(AttackZone.Last()));
	
	dx = FMath::RandRange(minDx, maxDx);
	dy = FMath::RandRange(minDy, maxDy);
	x = targetLoc.X + dx;
	y = targetLoc.Y + dy;
	
	AttackZone.Add(std::make_pair(FVector(x, y, getFloorZ(FVector2D(x, y))), PoundZoneRadius));
	DecalZone.Add(DrawAttackZoneDecal(AttackZone.Last()));

	dx = FMath::RandRange(minDx, maxDx);
	dy = FMath::RandRange(minDy, maxDy);
	x = targetLoc.X + dx;
	y = targetLoc.Y + dy;
	
	AttackZone.Add(std::make_pair(FVector(x, y, getFloorZ(FVector2D(x, y))), PoundZoneRadius));
	DecalZone.Add(DrawAttackZoneDecal(AttackZone.Last()));
}

void UAwakenThorFSM::StartPoundAttack()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), Me->PoundCameraShake, Me->GetActorLocation(), 0, 15000);
	SpawnRandomThunderVFX(AttackZone.Top().first);
	SphereOverlap(AttackZone.Pop(), 10, EHitType::STUN, false);
	DecalZone.Pop()->DestroyComponent();
}

void UAwakenThorFSM::StartClapAttack()
{
	FVector attackLoc = Me->GetMesh()->GetBoneLocation(FName("LeftHand"));

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ClapVFX,
		attackLoc
		);
	SphereOverlap(std::make_pair(attackLoc, ClapZoneRadius), 10, EHitType::NB_HIGH, true);
}

void UAwakenThorFSM::StartKickAttack()
{
	FVector attackLoc = Me->GetMesh()->GetBoneLocation(FName("RightFoot"));
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ClapVFX,
			attackLoc
			);
	SphereOverlap(std::make_pair(attackLoc, KickZoneRadius), 10, EHitType::NB_HIGH, true);
}

void UAwakenThorFSM::ReadyJumpAttack()
{
	FVector org = Me->GetActorLocation();
	FVector fwd = Me->GetActorForwardVector();
	org.Z = getFloorZ(FVector2D(org.X, org.Y));

	AttackZone.Empty();
	for (int i = 0; i < 5; i++)
	{
		FVector tmp = org + (fwd + FRotator(0, 75 * i, 0).Vector()) * 1000;
		tmp.Z = getFloorZ(FVector2D(tmp.X, tmp.Y));
		AttackZone.Add(std::make_pair(tmp, JumpAtkZoneRaidus));
		DecalZone.Add(DrawAttackZoneDecal(AttackZone.Last()));
	}
}

void UAwakenThorFSM::StartJumpAttack()
{
	while (!AttackZone.IsEmpty())
	{
		SpawnRandomThunderVFX(AttackZone.Top().first);
		SphereOverlap(AttackZone.Pop(), 10, EHitType::STUN, false);
		DecalZone.Pop()->DestroyComponent();
	}
	
	FVector loc = Me->GetActorLocation();
	FVector target = Target->GetActorLocation();
	
	Me->SetActorLocation(FVector(target.X, target.Y, loc.Z) + Target->GetActorForwardVector() * 100);
	FVector newLoc = Me->GetActorLocation();
	newLoc.Z = getFloorZ(FVector2D(newLoc.X, newLoc.Y));
	AttackZone.Empty();
	AttackZone.Add(std::make_pair(newLoc, JumpAtkZoneRaidus));
	DecalZone.Add(DrawAttackZoneDecal(AttackZone.Last()));
}

void UAwakenThorFSM::StartFallAttack()
{
	UGameplayStatics::PlayWorldCameraShake(GetWorld(), Me->PoundCameraShake, Me->GetActorLocation(), 0, 15000);
	SpawnThunderVFX(AttackZone.Top().first);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ThunderVFX, AttackZone.Top().first - FVector(0, 0, 100));
	SphereOverlap(AttackZone.Pop(),20, EHitType::NB_HIGH, false);
	DecalZone.Pop()->DestroyComponent();
}

void UAwakenThorFSM::GetHitDirectionString(EAttackDirectionType AtkDir, FString& Str)
{
	Str = UEnum::GetValueAsString(AtkDir).Mid(22);
}

void UAwakenThorFSM::OnEnd()
{
	bSuperArmor = false;
	if (!DecalZone.IsEmpty())
	{
		for (auto zone: DecalZone)
			zone->DestroyComponent();
		DecalZone.Empty();
	}
	if (!AttackZone.IsEmpty())
	{
		AttackZone.Empty();
	}
	if (State == EAwakenThorState::LeftTeleport || State == EAwakenThorState::RightTeleport)
		State = EAwakenThorState::BackTeleport;
	else if (State == EAwakenThorState::Teleport)
		State = EAwakenThorState::RangedAttackChange;
	else
		State = EAwakenThorState::Idle;
}

void UAwakenThorFSM::SetDamage(float Damage, EAttackDirectionType AtkDir, bool bSuperAttack)
{
	if (State == EAwakenThorState::Die)
		return ;
	
	bool isDie = Me->SetHp(Damage);

	if (GameMode)
		GameMode->SetEnemyHpBar(Me->GetHpPercent());

	if (isDie)
	{
		State = EAwakenThorState::Die;
		if (GameMode)
			GameMode->EndWithSucceed();
		return;
	}

	if (bSuperArmor && !bSuperAttack)
		return ;
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

void UAwakenThorFSM::SetSuperArmor(bool Value)
{
	bSuperArmor = Value;
}

EAwakenThorState UAwakenThorFSM::GetState() const
{
	return State;
}

void UAwakenThorFSM::SphereOverlap(const std::pair<FVector, float>& Zone, float Damage, EHitType HitType, bool IsMelee)
{
	TArray<AActor *> OverlappedActors;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
	
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Zone.first,
		Zone.second,
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
}

float UAwakenThorFSM::getFloorZ(FVector2D Loc)
{
	FVector start = FVector(Loc.X, Loc.Y, 1000);
	FVector end = start + FVector::DownVector * 1000;

	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(Me);
	params.AddIgnoredActor(Target);

	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, params))
		return hitResult.ImpactPoint.Z;
	return 0;
}

UDecalComponent* UAwakenThorFSM::DrawAttackZoneDecal(const std::pair<FVector, float>& Zone)
{
	FVector size = FVector(40, Zone.second,Zone.second);
	UMaterialInterface* AttackZoneDecal = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Material'/Game/CSW/Material/AttackZone1.AttackZone1'"));
	
	return UGameplayStatics::SpawnDecalAtLocation(GetWorld(), AttackZoneDecal, size, Zone.first - FVector(0, 0, 50), FRotator(-90, 0, 0));
}

void UAwakenThorFSM::SpawnRandomThunderVFX(const FVector& Location)
{
	if (ThunderVFXarr.IsEmpty())
		return ;
	
	int rand = FMath::RandRange(0, ThunderVFXarr.Num() - 1);

	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ThunderVFXarr[rand],
		Location - FVector(0, 0, 20)
		);

}

void UAwakenThorFSM::SpawnThunderVFX(const FVector& Location)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ThunderVFX,
		Location - FVector(0, 0, 20)
		);
}

