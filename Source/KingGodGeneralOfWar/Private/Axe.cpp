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

const float AXE_DAMAGE = 3;
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
}

// Called every frame
void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAxe::OnAxeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFXFactory, EdgeComp->GetComponentTransform());
	auto* Thor = Cast<ABDThor>(OtherActor);
	auto* player = Cast<AKratos>(GetAttachParentActor());

	if (Thor)
	{
		int attackNum = 0;
		if (player->CurrentAttackType == EAttackType::WEAK_ATTACK || player->CurrentAttackType == EAttackType::RUNE_ATTACK)
			attackNum = player->CurrentWeakCombo;
		else if (player->CurrentAttackType == EAttackType::STRONG_ATTACK)
			attackNum = player->CurrentStrongCombo;
		else
			attackNum = 0;
		EAttackDirectionType attackDirection = AttackTypeDirectionArr[static_cast<int8>(player->CurrentAttackType)][attackNum];
		Thor->fsm->Damage(AXE_DAMAGE);
	}
	else
	{
		auto AwakenThor = Cast<AAwakenThor>(OtherActor);
		
		int attackNum = 0;
		if (player->CurrentAttackType == EAttackType::WEAK_ATTACK || player->CurrentAttackType == EAttackType::RUNE_ATTACK)
			attackNum = player->CurrentWeakCombo;
		else if (player->CurrentAttackType == EAttackType::STRONG_ATTACK)
			attackNum = player->CurrentStrongCombo;
		else
			attackNum = 0;
		EAttackDirectionType attackDirection = AttackTypeDirectionArr[static_cast<int8>(player->CurrentAttackType)][attackNum];
		AwakenThor->getFSM()->SetDamage(AXE_DAMAGE, attackDirection);
	}

	if (player)		player->CameraShakeOnAttack(0.5);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.08f);
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, 0.008f, false);
}


