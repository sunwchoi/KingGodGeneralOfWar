// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"
#include "components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Kratos.h"
#include "CSW/AwakenThor.h"
#include "CSW/AwakenThorFSM.h"
#include "BDThor.h"
#include "BDThorFSM.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FlyingAxe.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

const float AXE_DAMAGE = 3;
const int BLOOD_VFX_MAX = 7;
// Sets default values
AAxe::AAxe()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	EdgeComp = CreateDefaultSubobject<UArrowComponent>(TEXT("EdgeComp"));
	EdgeComp->SetupAttachment(MeshComp);
	EdgeComp->SetRelativeLocation(FVector(-30, 0, 40));
	EdgeComp->SetRelativeScale3D(FVector(.4, .4, .4));

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->SetUpdatedComponent(RootComponent);
	MovementComp->Velocity = FVector(0, 0, 0);
}

// Called when the game starts or when spawned
void AAxe::BeginPlay()
{
	Super::BeginPlay();
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AAxe::OnAxeBeginOverlap);
	BloodVFXFactoryArr.Add(BloodVFX1Factory);
	BloodVFXFactoryArr.Add(BloodVFX2Factory);
	BloodVFXFactoryArr.Add(BloodVFX3Factory);
	BloodVFXFactoryArr.Add(BloodVFX4Factory);
	BloodVFXFactoryArr.Add(BloodVFX5Factory);
	BloodVFXFactoryArr.Add(BloodVFX6Factory);
	BloodVFXFactoryArr.Add(BloodVFX7Factory);
	BloodVFXFactoryArr.Add(BloodVFX8Factory);
	
}

// Called every frame
void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 도끼 공격이 닿았을 때 데미지와 공격 방향을 전달
void AAxe::OnAxeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleWeapon"), true);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodVFXFactoryArr[FMath::RandRange(0, BLOOD_VFX_MAX)], EdgeComp->GetComponentLocation());
	auto* Thor = Cast<ABDThor>(OtherActor);
	EAttackDirectionType attackDirection = EAttackDirectionType::UP;
	if (Thor)
	{
		attackDirection = Me->GetAttackDirection();
		Thor->fsm->Damage(AXE_DAMAGE, attackDirection);
	}
	else
	{
		auto AwakenThor = Cast<AAwakenThor>(OtherActor);
		if (AwakenThor)
		{
			attackDirection = Me->GetAttackDirection();
			AwakenThor->getFSM()->SetDamage(AXE_DAMAGE, attackDirection);
		}
	}

	if (Me->CurrentAttackType == EAttackType::WEAK_ATTACK)
	{
		int32 curCombo = Me->GetCurrentWeakCombo();
		if (curCombo >= 3)
		{
			Me->CameraShakeOnAttack(attackDirection, 1.0f);
		}
		else
		{
		}
		Me->SetGlobalTimeDilation(0.004f, 0.04f);

	}
	else if (Me->CurrentAttackType == EAttackType::STRONG_ATTACK)
	{
		Me->CameraShakeOnAttack(attackDirection, 1.1f);
		Me->SetGlobalTimeDilation(0.004f, 0.04f);

	}
	else if (Me->CurrentAttackType == EAttackType::RUNE_ATTACK)
	{
		Me->CameraShakeOnAttack(attackDirection, 1.2f);
		Me->SetGlobalTimeDilation(0.004f, 0.04f);

	}

}


