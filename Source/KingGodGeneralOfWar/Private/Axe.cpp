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
	MeshComp->OnComponentHit.AddDynamic(this, &AAxe::OnAxeHit);
}

// Called every frame
void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAxe::OnAxeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	MeshComp->UPrimitiveComponent::SetCollisionProfileName(TEXT("IdleAxe"), true);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01f);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFXFactory, EdgeComp->GetComponentTransform());
	auto* Thor = Cast<ABDThor>(OtherActor);
	if (Thor)
	{
		Thor->fsm->Damage(10);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Black, TEXT("Thor Hit"));
	}
	else
	{
		auto AwakenThor = Cast<AAwakenThor>(OtherActor);
		AwakenThor->getFSM()->SetDamage();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Black, TEXT("AwakenThor Hit"));
	}

	auto* player = Cast<AKratos>(GetOwner());
	if (player)		player->CameraShakeOnAttack();

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		}, 0.001f, false);
}

void AAxe::OnAxeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Hit.BoneName.ToString());
}


